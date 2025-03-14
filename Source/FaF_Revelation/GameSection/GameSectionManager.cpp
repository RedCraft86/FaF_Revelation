// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameSectionManager.h"
#include "Framework/ToroWidgetManager.h"
#include "MusicSystem/ToroMusicManager.h"
#include "FaF_Revelation/FaFRevSettings.h"
#include "SaveSystem/ToroSaveManager.h"
#include "Engine/LevelScriptActor.h"
#include "ToroShortcutLibrary.h"

void UGameSectionManager::StepSequence(const uint8 InIndex, const FGameplayTag SaveTag)
{
	if (bLoading || !Graph) return;
	
	Sequence.Add(InIndex);
	Sequence = Graph->ValidateSequence(Sequence);
	ChangeSection(Graph->GetLeafInSequence<UGameSectionNode>(Sequence, true), SaveTag);
	
	if (UFaFRevSaveObject* Save = GetGameSave(SaveTag))
	{
		Save->Sequence = Sequence;
	}
}

void UGameSectionManager::LoadSequence(const FGameplayTag SaveTag)
{
	if (!Graph) return;
	if (const UFaFRevSaveObject* Save = GetGameSave(SaveTag))
	{
		Sequence = Graph->ValidateSequence(Save->Sequence);
		ChangeSection(Graph->GetLeafInSequence<UGameSectionNode>(Sequence, true), SaveTag);
	}
}

void UGameSectionManager::ChangeSection(UGameSectionNode* NewSection, const FGameplayTag SaveTag)
{
	if (bLoading || !NewSection || NewSection == Section
		|| !Graph || !Graph->AllNodes.Contains(NewSection)) return;
	
	bLoading = true;
	bWaiting = false;
	bMainLevelLoaded = false;
	
	if (AToroPlayerBase* Player = AToroPlayerBase::Get(this))
	{
		Player->EnterCinematic(GetWorld()->GetLevelScriptActor());
		Player->AddLockFlag(GTag_LockLoading);
	}

	ToLoad = NewSection->GetLevels();
	
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
	else
	{
		NewSection->PlayStart();
	}
	
	for (const TSoftObjectPtr<UWorld>& Level : ToLoad)
	{
		LoadLevel(Level);
	}

	if (const AToroGameMode* GameMode = AToroGameMode::Get(this))
	{
		if (Section) GameMode->Narrative->ForgetQuest(Section->Quest.LoadSynchronous());
		GameMode->Narrative->BeginQuest(NewSection->Quest.LoadSynchronous());

		if (UFaFRevSaveObject* Save = GetGameSave(SaveTag))
		{
			if (Section)
			{
				Save->Inventory.Add(NewSection->NodeID, GameMode->Inventory->GetSaveData());
			}
			else
			{
				GameMode->Inventory->SetSaveData(Save->Inventory.FindRef(NewSection->NodeID));
			}
		}

		GameMode->Inventory->EnsureItems(Section->Inventory);
	}

	if (UGlobalSaveObjectBase* Save = GetGlobalSave())
	{
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

	if (UDSSetter) UDSSetter->SetCustomSettings(this, Section->SkyWeather);
	if (AToroMusicManager* MusicManager = AToroMusicManager::Get(this))
	{
		MusicManager->ChangeMainTheme(Section->Theme.LoadSynchronous());
	}

	if (AToroPlayerBase* Player = AToroPlayerBase::Get(this))
	{
		Player->OverrideControlFlags(Section->PlayerControl);
		Player->SetLightSettings(Section->PlayerLight);
	}
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

	if (AToroPlayerBase* Player = AToroPlayerBase::Get(this))
	{
		Player->ExitCinematic();
		Player->ClearLockFlag(GTag_LockLoading);
	}

	bLoading = false;
}

UGlobalSaveObjectBase* UGameSectionManager::GetGlobalSave()
{
	if (GlobalSave) return GlobalSave;
	GlobalSave = ToroSave::GetObject<UGlobalSaveObjectBase>(this, GTag_GlobalSave);
	return GlobalSave;
}

UFaFRevSaveObject* UGameSectionManager::GetGameSave(const FGameplayTag& SaveTag)
{
	const FGameplayTag Tag = SaveTag.IsValid() && SaveTag != GAMEPLAY_TAG(Saves) ? SaveTag : GTag_GameSave;
	
	if (LastSaveTag == Tag && GameSave)
		return GameSave;

	LastSaveTag = Tag;
	GameSave = ToroSave::GetObject<UFaFRevSaveObject>(this, Tag);
	return GameSave;
}

bool UGameSectionManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && UToroRuntimeSettings::Get()->IsOnGameplayMap(Outer);
}

bool UGameSectionManager::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE ||
		WorldType == EWorldType::GamePreview || WorldType == EWorldType::GameRPC;
}

void UGameSectionManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Graph = UFaFRevSettings::Get()->SectionGraph.LoadSynchronous();
}

void UGameSectionManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	if (const TSubclassOf<UUDSSetterObject> Class = UFaFRevSettings::Get()->UDS_Setter.LoadSynchronous())
	{
		UDSSetter = NewObject<UUDSSetterObject>(this, Class);
	}
}
