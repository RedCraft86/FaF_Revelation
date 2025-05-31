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
#define GetWorldTimerManager GetWorld()->GetTimerManager

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
}

void UGamePhaseManager::OnStartSequenceEnd()
{
}

void UGamePhaseManager::OnFinishSequenceEnd()
{
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
