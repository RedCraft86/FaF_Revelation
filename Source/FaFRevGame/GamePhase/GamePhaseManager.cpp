// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePhaseManager.h"
#include "Engine/LevelScriptActor.h"
#include "Framework/ToroWidgetManager.h"
#include "MiscClasses/MasterPostProcess.h"
#include "MusicSystem/GameMusicManager.h"
#include "Libraries/ToroGeneralUtils.h"
#include "Helpers/LatentInfo.h"
#include "FaFRevSettings.h"
#include "SaveObjects.h"

#define NarrativeRef PlayerChar->Narrative
#define InventoryRef PlayerChar->Inventory
#define GetWorldTimerManager GetWorld()->GetTimerManager

#define GET_SAVE(Type, ...) \
	if (U##Type##SaveObject* Save = SaveManager ? SaveManager->GetSaveObject<U##Type##SaveObject>(GAMEPLAY_TAG_CHILD(Save, Type)) : nullptr) \
		__VA_ARGS__

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

void UGamePhaseManager::LoadSequence()
{
	if (!bLoading && PlayerChar && Graph)
	{
		GET_SAVE(Game, {
			Sequence = Graph->ValidateSequence(Save->Sequence);
			ChangePhase(Graph->GetLeafInSequence<UGamePhaseNode>(Sequence, true));
		})
	}
}

void UGamePhaseManager::ChangePhase(UGamePhaseNode* NewPhase)
{
	if (!bLoading && PlayerChar && NewPhase && NewPhase != ThisPhase && Graph && Graph->AllNodes.Contains(NewPhase))
	{
		bLoading = true;
		bWaiting = false;
		bMainLoaded = false;

		PlayerChar->EnterCinematic(GetWorld()->GetLevelScriptActor());
		PlayerChar->AddLockFlag(GAMEPLAY_TAG_CHILD(PlayerLock, Loading));
		
		ToLoad = NewPhase->GetLevels();
		ToUnload = ThisPhase ? ThisPhase->GetLevels().Difference(ToLoad) : TSet<TSoftObjectPtr<UWorld>>{};

		if (ThisPhase) ThisPhase->UnbindSequenceEvents();
		NewPhase->BindSequenceEvents(this);

		if (NewPhase->StartSequence.LoadSynchronous() && ThisPhase) NewPhase->PlayStartSequence();
		else // Show the UI instead and mark waiting if we don't have a start sequence, or we're jumping to this phase
		{
			bWaiting = true;
			ShowLoadUI();
		}

		for (const TSoftObjectPtr<UWorld>& Level : ToLoad)
		{
			LoadLevel(Level);
		}

		if (ThisPhase) NarrativeRef->ForgetQuest(ThisPhase->Quest.LoadSynchronous());
		NarrativeRef->BeginQuest(NewPhase->Quest.LoadSynchronous());

		GET_SAVE(Global, {
			Save->SectionNodes.Add(NewPhase->NodeID);
			Save->Content.Append(NewPhase->UnlockContent);
		})

		GET_SAVE(Game, {
			if (ThisPhase)
			{
				// If we have a previous phase, we will assume that the player JUST unlocked this new phase
				// So we will save the inventory data into the new phase
				Save->Inventory.Add(NewPhase->NodeID, InventoryRef->GetSaveData());
			}
			else
			{
				// If we don't have one, it implies we might be loading the game or the player used the phase selector
				// In that case, we should already have a previously saved inventory to load from
				InventoryRef->LoadSaveData(Save->Inventory.FindRef(NewPhase->NodeID));
			}
		})

		// Ensure certain absolutely necessary items/archive entries for this phase are in the inventory
		InventoryRef->EnsureInventory(NewPhase->Equipment, NewPhase->Items, NewPhase->Archives);

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
		bMainLoaded = true;
		ThisPhase->TeleportPlayer();

		//PlayerChar->OverrideControlFlags(ThisPhase->PlayerControl); // TODO
		PlayerChar->SetLightSettings(ThisPhase->PlayerLight);

		if (AGameMusicManager* MusicManager = AGameMusicManager::Get<AGameMusicManager>(this))
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
		if (!bMainLoaded)
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
	GetWorldTimerManager().ClearTimer(WidgetTimer);
	UnloadLevels();
	HideLoadUI();

	PlayerChar->ExitCinematic();
	PlayerChar->ClearLockFlag(GAMEPLAY_TAG_CHILD(PlayerLock, Loading));

	bWaiting = false;
	bLoading = false;
}

void UGamePhaseManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	PlayerChar = AGamePlayer::Get<AGamePlayer>(this);
}

void UGamePhaseManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SaveManager = UToroSaveManager::Get(this);
	Graph = UFaFRevSettings::Get()->PhaseGraph.LoadSynchronous();
	if (AToroWidgetManager* WidgetManager = AToroWidgetManager::Get(this))
	{
		LoadingUI = Graph ? WidgetManager->FindOrAddWidget<ULoadingWidgetBase>(Graph->LoadingWidget) : nullptr;
	}
}
