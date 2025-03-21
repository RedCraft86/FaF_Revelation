// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameSectionManager.h"
#include "Framework/ToroWidgetManager.h"
#include "Framework/ToroMusicManager.h"
#include "SaveSystem/ToroSaveManager.h"
#include "Engine/LevelScriptActor.h"
#include "ToroShortcutLibrary.h"
#include "GameSettings.h"
#include "ToroSettings.h"

#define InventoryRef Player->GetInventory()
#define NarrativeRef Player->GetNarrative()

void UGameSectionManager::StepSequence(const uint8 InIndex)
{
	if (bLoading || !Graph)
		return;

	Sequence.Add(InIndex);
	Sequence = Graph->ValidateSequence(Sequence);
	ChangeSection(Graph->GetLeafInSequence<UGameSectionNode>(Sequence, true));
	if (UGameSaveObject* Save = GetGameSave())
	{
		Save->Sequence = Sequence;
	}
}

void UGameSectionManager::LoadSequence()
{
	if (!Graph) return;
	if (const UGameSaveObject* Save = GetGameSave())
	{
		Sequence = Graph->ValidateSequence(Save->Sequence);
		ChangeSection(Graph->GetLeafInSequence<UGameSectionNode>(Sequence, true));
	}
}

void UGameSectionManager::ChangeSection(UGameSectionNode* NewSection)
{
	if (bLoading || !NewSection || NewSection == Section
		|| !Graph || !Graph->AllNodes.Contains(NewSection)) return;

	bLoading = true;
	bWaiting = false;
	bMainLevelLoaded = false;
	ToLoad = NewSection->GetLevels();

	Player->EnterCinematic(GetWorld()->GetLevelScriptActor());
	Player->AddLockFlag(GAMEPLAY_TAG_CHILD(Loading, PlayerLock));

	ToUnload.Empty();
	TSet<TSoftObjectPtr<UWorld>> OldLevels = Section ? Section->GetLevels() : TSet<TSoftObjectPtr<UWorld>>{};
	for (const TSoftObjectPtr<UWorld>& Level : OldLevels)
	{
		if (!Level.IsNull() && !ToLoad.Contains(Level)) ToUnload.Add(Level);
	}

	if (ToUnload.IsEmpty() || !NewSection->StartSequence)
	{
		SetWidgetHidden(false);
		UToroShortcutLibrary::SetCameraFade(this, 1.0f, FLinearColor::Black, true);
	}
	else NewSection->PlayStart();

	for (const TSoftObjectPtr<UWorld>& Level : ToLoad)
	{
		LoadLevel(Level);
	}

	if (Section) NarrativeRef->ForgetQuest(Section->Quest.LoadSynchronous());
	NarrativeRef->BeginQuest(NewSection->Quest.LoadSynchronous());
	if (UGameSaveObject* Save = GetGameSave())
	{
		if (Section)
		{
			Save->Inventory.Add(NewSection->NodeID, InventoryRef->GetSaveData());
		}
		else
		{
			InventoryRef->SetSaveData(Save->Inventory.FindRef(NewSection->NodeID));
		}
	}
	InventoryRef->EnsureItems(Section->Inventory);

	if (UGlobalSaveObject* Save = GetGlobalSave())
	{
		Save->SectionNodes.Add(NewSection->NodeID);
		Save->Content.Append(NewSection->UnlockContent);
	}

	Section = NewSection;
}

void UGameSectionManager::SetWidgetHidden(const bool bInHidden)
{
	if (!Widget)
	{
		if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
		{
			Widget = Manager->FindWidget<ULoadingWidgetBase>();
		}
	}
	if (Widget)
	{
		Widget->SetHidden(bInHidden);
	}
}

void UGameSectionManager::UnloadLevel(const TSoftObjectPtr<UWorld>& InLevel) const
{
	if (InLevel.IsNull()) return;
	FLatentActionInfo Info;
	Info.Linkage = 0;
	Info.UUID = GetLatentID();
	Info.CallbackTarget = nullptr;
	Info.ExecutionFunction = NAME_None;
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, InLevel, Info , false);
}

void UGameSectionManager::LoadLevel(const TSoftObjectPtr<UWorld>& InLevel)
{
	if (InLevel.IsNull() || !Section) return;
	const bool bMainLevel = InLevel == Section->MainLevel;
	const FName MapName(*FPackageName::ObjectPathToPackageName(InLevel.ToString()));
	const bool bShouldBeVisible = bMainLevel || Section->Levels.FindRef(InLevel);
	ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(this, MapName);
	if (Level && Level->IsLevelLoaded())
	{
		Level->SetShouldBeVisible(bShouldBeVisible);
		if (InLevel == Section->MainLevel) OnMainLevelLoaded();
		return;
	}

	FLatentActionInfo Info;
	Info.Linkage = 0;
	Info.UUID = GetLatentID();
	Info.CallbackTarget = bMainLevel ? this : nullptr;
	Info.ExecutionFunction = bMainLevel ? FName(TEXT("OnMainLevelLoaded")) : NAME_None;
	UGameplayStatics::LoadStreamLevel(this, MapName, bShouldBeVisible, false, Info);
}

void UGameSectionManager::OnMainLevelLoaded()
{
	if (!Section) return;
	bMainLevelLoaded = true;
	if (ToUnload.IsEmpty() || !Section->StartSequence)
	{
		Section->Teleport->TeleportPlayer();
		UToroShortcutLibrary::StartCameraFade(this, 1.0f, 0.0f, 2.0f);
		OnEndSequenceFinished();
	}
	else if (bWaiting)
	{
		Section->StopWait();
		if (!Section->PlayEnd())
			OnEndSequenceFinished();
	}

	if (AToroMusicManager* MusicManager = AToroMusicManager::Get(this))
	{
		MusicManager->ChangeMainTheme(Section->Theme.LoadSynchronous());
	}

	UGameSettings::Get()->SetUDSSettings(this, Section->SkyWeather);
	Player->OverrideControlFlags(Section->PlayerControl);
	Player->SetLightSettings(Section->PlayerLight);
}

void UGameSectionManager::OnStartSequenceFinished()
{
	if (!Section) return;
	Section->Teleport->TeleportPlayer();
	if (bMainLevelLoaded)
	{
		if (!Section->PlayEnd())
			OnEndSequenceFinished();
	}
	else
	{
		bWaiting = true;
		Section->PlayWait();
		GetWorld()->GetTimerManager().SetTimer(WidgetTimer, this, &ThisClass::DelayedShowWidget,
			FMath::Max(0.05f, Section->WidgetDelay), false);
	}
}

void UGameSectionManager::OnEndSequenceFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(WidgetTimer);
	SetWidgetHidden(true);

	for (const TSoftObjectPtr<UWorld>& Level : ToUnload)
	{
		UnloadLevel(Level);
	}
	ToUnload.Empty();
	Player->ExitCinematic();
	Player->ClearLockFlag(GAMEPLAY_TAG_CHILD(Loading, PlayerLock));
	bLoading = false;
}

UGlobalSaveObject* UGameSectionManager::GetGlobalSave()
{
	if (GlobalSave) return GlobalSave;
	GlobalSave = ToroSave::GetObject<UGlobalSaveObject>(this, GAMEPLAY_TAG_CHILD(Global, Save));
	return GlobalSave;
}

UGameSaveObject* UGameSectionManager::GetGameSave() const
{
	if (UToroSaveManager* SaveManager = UToroSaveManager::Get(this))
	{
		return SaveManager->GetSaveObject<UGameSaveObject>(SaveManager->ActiveGameSave);
	}
	return nullptr;
}

bool UGameSectionManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && UToroSettings::Get()->IsOnGameplayMap(Outer);
}

bool UGameSectionManager::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE ||
		WorldType == EWorldType::GamePreview || WorldType == EWorldType::GameRPC;
}

void UGameSectionManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Graph = UGameSettings::Get()->SectionGraph.LoadSynchronous();
}

void UGameSectionManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	Player = AToroPlayerCharacter::Get<AGamePlayerBase>(this);
}
