// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePhaseManager.h"
#include "Engine/LevelScriptActor.h"
#include "Framework/ToroWidgetManager.h"
#include "MiscClasses/MasterPostProcess.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "Libraries/ToroGeneralUtils.h"
#include "Helpers/LatentInfo.h"
#include "FaFRevSettings.h"
#include "SaveObjects.h"

#define NarrativeRef PlayerChar->Narrative
#define InventoryRef PlayerChar->Inventory

#define GET_SAVE(Type, ...) \
	if (U##Type##SaveObject* Save = SaveManager ? SaveManager->GetSaveObject<U##Type##SaveObject>(Tag_Save_##Type##) : nullptr) \
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
		bLevelLoaded = false;

		PlayerChar->EnterCinematic(GetWorld()->GetLevelScriptActor());
		PlayerChar->AddLockFlag(GAMEPLAY_TAG_CHILD(PlayerLock, Loading));

		LevelsToLoad = NewPhase->GetLevels();
		LevelsToUnload = ThisPhase ? ThisPhase->GetLevels().Difference(LevelsToLoad) : TSet<TSoftObjectPtr<UWorld>>{};

		if (ThisPhase)
		{
			ThisPhase->UnbindSequenceEvents();
			NarrativeRef->ForgetQuest(ThisPhase->Quest.LoadSynchronous());
		}

		NewPhase->BindSequenceEvents(this);
		NarrativeRef->BeginQuest(NewPhase->Quest.LoadSynchronous());

		if (LevelsToUnload.IsEmpty() || !NewPhase->StartSequence.LoadSynchronous())
		{
			SetLoadingUIHidden(false); // Show Loading UI if we don't have a start sequence or jumping in
			UToroShortcutLibrary::SetCameraFade(this, 1.0f, FLinearColor::Black, true);
		}
		else NewPhase->PlayStartSequence();

		for (const TSoftObjectPtr<UWorld>& Level : LevelsToLoad)
		{
			LoadLevel(Level);
		}

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

		GET_SAVE(Global, {
			Save->SectionNodes.Add(NewPhase->NodeID); // Used for Phase Selector to know which phases are unlocked
			Save->Content.Append(NewPhase->UnlockContent);
		})

		// We don't care about the last phase anymore
		ThisPhase = NewPhase;
	}
}

void UGamePhaseManager::SetLoadingUIHidden(const bool bInHidden) const
{
	if (LoadingUI) LoadingUI->SetHidden(bInHidden);
}

void UGamePhaseManager::UnloadLevel(const TSoftObjectPtr<UWorld>& InLevel) const
{
	if (InLevel.IsNull()) return;
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, InLevel, LatentInfo::Make() , false);
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
				OnMainLevelLoaded();
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
		bLevelLoaded = true;
		if (LevelsToUnload.IsEmpty() || !ThisPhase->StartSequence.LoadSynchronous())
		{
			UToroShortcutLibrary::StartCameraFade(this, 1.0f, 0.0f, 2.0f);
			ThisPhase->TeleportPlayer();
			OnFinishSequenceEnd();
		}
		else if (bWaiting)
		{
			if (!ThisPhase->PlayFinishSequence())
				OnFinishSequenceEnd();
		}

		// if ()
		// {
		// 	TODO Music system
		// }

		if (AMasterPostProcess* PP = AMasterPostProcess::Get(this))
		{
			PP->SetUDSSettings(ThisPhase->SkyWeather);
		}

		//PlayerChar->OverrideControlFlags(ThisPhase->PlayerControl); // TODO
		PlayerChar->SetLightSettings(ThisPhase->PlayerLight);
	}
}

void UGamePhaseManager::OnStartSequenceEnd()
{
	if (ThisPhase)
	{
		ThisPhase->TeleportPlayer();
		if (!bLevelLoaded)
		{
			bWaiting = true;
			GetWorld()->GetTimerManager().SetTimer(WidgetTimer, this, &ThisClass::ShowLoadUIFunc,
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
	GetWorld()->GetTimerManager().ClearTimer(WidgetTimer);
	SetLoadingUIHidden(true);
	bWaiting = false;

	for (const TSoftObjectPtr<UWorld>& Level : LevelsToUnload)
	{
		UnloadLevel(Level);
	}
	LevelsToUnload.Empty();

	UToroGeneralUtils::ForceGarbageCollection();

	PlayerChar->ExitCinematic();
	PlayerChar->ClearLockFlag(GAMEPLAY_TAG_CHILD(PlayerLock, Loading));
	bLoading = false;
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

void UGamePhaseManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	PlayerChar = AGamePlayer::Get<AGamePlayer>(this);
}
