// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePhaseManager.h"
#include "FaFRevGameInstance.h"
#include "Engine/LevelScriptActor.h"
#include "Libraries/ToroGeneralUtils.h"
#include "Framework/ToroWidgetManager.h"
#include "MusicSystem/GameMusicManager.h"
#include "Narrative/ToroNarrative.h"
#include "Helpers/LatentInfo.h"
#include "FaFRevSettings.h"
#include "FaFRevSaves.h"

#define NarrativeRef PlayerChar->Narrative
#define InventoryRef PlayerChar->Inventory
#define GetWorldTimerManager GetWorld()->GetTimerManager

#define GET_SAVE(Type, ...) \
	if (UFaFRev##Type##Save* Save = UToroSaveManager::GetSaveObject<UFaFRev##Type##Save>(this, SaveTags::TAG_##Type)) \
		__VA_ARGS__

void UGamePhaseManager::LoadSequence()
{
	if (!bLoading && PlayerChar && Graph)
	{
		UFaFRevGameInstance* GI = UToroGameInstance::Get<UFaFRevGameInstance>(this);
		if (GI && GI->PlayFromPhase.IsSet())
		{
			ChangePhase(Cast<UGamePhaseNode>(Graph->GetNodeByID(*GI->PlayFromPhase)));
			GI->PlayFromPhase.Reset();
		}
		else
		{
			GET_SAVE(Game, {
				Sequence = Graph->ValidateSequence(Save->Sequence);
				ChangePhase(Graph->GetLeafInSequence<UGamePhaseNode>(Sequence, true));
			})
		}
	}
}

void UGamePhaseManager::StepSequence(const uint8 InIndex)
{
	if (!bLoading && PlayerChar && Graph)
	{
		Sequence.Add(InIndex);
		Sequence = Graph->ValidateSequence(Sequence);
		ChangePhase(Graph->GetLeafInSequence<UGamePhaseNode>(Sequence, true));
		GET_SAVE(Game, {
			Save->Sequence = Sequence;
		})
	}
}

void UGamePhaseManager::ChangePhase(UGamePhaseNode* NewPhase)
{
	if (bLoading || !PlayerChar) return;
	if (!NewPhase || NewPhase == ThisPhase || !Graph || !Graph->AllNodes.Contains(NewPhase)) return;
	if (UFaFRevGameSave* GameSave = UToroSaveManager::GetSaveObject<UFaFRevGameSave>(this, SaveTags::TAG_Game))
	{
		bLoading = true;
		bWaiting = false;
		bMainLoad = false;

		PlayerChar->AddLockTag(PlayerLockTag::TAG_Loading);
		PlayerChar->EnterCinematic(GetWorld()->GetLevelScriptActor());

		ToLoad = NewPhase->GetLevels();
		ToUnload = ThisPhase ? ThisPhase->GetLevels().Difference(ToLoad) : TSet<TSoftObjectPtr<UWorld>>();

		if (ThisPhase)
		{
			ThisPhase->UnbindSequenceEvents();
			NarrativeRef->ForgetQuest(ThisPhase->Quest.LoadSynchronous());
			GameSave->SetPhasePlayTime(ThisPhase->NodeID, PhaseTime);

			// This is either a new phase or is worth updating, save the inventory.
			GameSave->SetPhaseInventory(NewPhase->NodeID, InventoryRef->GetSaveData());
		}
		else
		{
			// We're definitely jumping in to a sequence without any past history. Only load inventory.
			InventoryRef->LoadSaveData(GameSave->GetPhaseInventory(NewPhase->NodeID));
		}

		NewPhase->BindSequenceEvents(this);
		InventoryRef->EnsureInventory(NewPhase->Equipment, NewPhase->Items, NewPhase->Archives);

		if (ThisPhase && NewPhase->StartSequence.LoadSynchronous())
		{
			NewPhase->PlayStartSequence();
		}
		else // Show the UI instead and mark waiting if we don't have a start sequence, or we're jumping to this phase
		{
			bWaiting = true;
			ShowLoadUI();
		}

		for (const TSoftObjectPtr<UWorld>& Level : ToLoad)
		{
			LoadLevel(Level);
		}

		GameSave->SaveObject(nullptr);
		GET_SAVE(Global, {
			Save->PhaseNodes.Add(NewPhase->NodeID);
			Save->Content.Append(NewPhase->GetContentTags());
			Save->SaveObject(nullptr);
		})

		ThisPhase = NewPhase;
	}
}

void UGamePhaseManager::UnloadLevels()
{
	UnloadTasks = 0;
	for (const TSoftObjectPtr<UWorld>& Level : ToUnload)
	{
		if (Level.IsNull()) continue;
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, Level,
			LatentInfo::Make(TEXT("OnUnloadLevel"), this) , false);
		UnloadTasks++;
	}
	ToUnload.Empty();
}

void UGamePhaseManager::OnUnloadLevel()
{
	if (UnloadTasks > 0) UnloadTasks--;
	if (UnloadTasks == 0) UToroGeneralUtils::ForceGarbageCollection();
}

void UGamePhaseManager::LoadLevel(const TSoftObjectPtr<UWorld>& InLevel)
{
	if (!InLevel.IsNull() && ThisPhase)
	{
		const bool bMainLevel = InLevel == ThisPhase->MainLevel;
		const FName MapName(*FPackageName::ObjectPathToPackageName(InLevel.ToString()));
		const bool bVisible = bMainLevel || ThisPhase->Levels.FindRef(InLevel);

		// If we already have this level loaded, just make it visible/invisible
		ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(this, MapName);
		if (Level && Level->IsLevelLoaded())
		{
			Level->SetShouldBeVisible(bVisible);
			if (InLevel == ThisPhase->MainLevel)
			{
				FTimerHandle Handle;
				GetWorldTimerManager().SetTimer(Handle, this,
					&UGamePhaseManager::OnMainLevelLoaded, 1.0f, false);
			}
			return;
		}

		// If it's the main map, it should make a callback to wrap up
		UGameplayStatics::LoadStreamLevel(this, MapName, bVisible, false,
			bMainLevel ? LatentInfo::Make(TEXT("OnMainLevelLoaded"), this) : LatentInfo::Make());
	}
}

void UGamePhaseManager::OnMainLevelLoaded()
{
	if (ThisPhase)
	{
		bMainLoad = true;
		ThisPhase->TeleportPlayer();

		PlayerChar->SetLightSettings(ThisPhase->PlayerLight);
		PlayerChar->OverrideControlFlags(ThisPhase->PlayerControl);

		if (UGameMusicManager* MusicManager = UGameMusicManager::Get(this))
		{
			MusicManager->ChangeMainTheme(ThisPhase->Theme.LoadSynchronous());
		}

		if (bWaiting && !ThisPhase->PlayFinishSequence())
		{
			OnFinishSequenceEnd();
		}
	}
}

void UGamePhaseManager::OnStartSequenceEnd()
{
	if (ThisPhase)
	{
		if (!bMainLoad)
		{
			bWaiting = true;
			GetWorldTimerManager().SetTimer(WidgetTimer, this, &UGamePhaseManager::ShowLoadUI,
				FMath::Max(0.05f, Graph->WidgetDelay), false);
		}
		else if (!ThisPhase->PlayFinishSequence())
		{
			OnFinishSequenceEnd();
		}
	}
}

void UGamePhaseManager::OnFinishSequenceEnd()
{
	NarrativeRef->BeginQuest(ThisPhase->Quest.LoadSynchronous());
	GetWorldTimerManager().ClearTimer(WidgetTimer);
	UnloadLevels();
	HideLoadUI();

	PlayerChar->ExitCinematic();
	PlayerChar->ClearLockTag(PlayerLockTag::TAG_Loading);

	bWaiting = false;
	bLoading = false;
}

void UGamePhaseManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bLoading) PhaseTime += DeltaTime;
}

void UGamePhaseManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		PlayerChar = AGamePlayerChar::Get<AGamePlayerChar>(this);
	});
}

void UGamePhaseManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Graph = UFaFRevSettings::Get()->PhaseGraph.LoadSynchronous();
	if (AToroWidgetManager* WidgetManager = AToroWidgetManager::Get(this))
	{
		LoadingUI = WidgetManager->FindWidget<UGameLoadingWidget>();
	}
}
