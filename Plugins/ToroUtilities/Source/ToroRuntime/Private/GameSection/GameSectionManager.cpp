// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameSection/GameSectionManager.h"
#include "Framework/ToroWidgetManager.h"
#include "SaveSystem/ToroSaveSystem.h"
#include "Engine/LevelScriptActor.h"
#include "ToroRuntimeSettings.h"
#include "ToroShortcutLibrary.h"

void UGameSectionManager::StepSequence(const uint8 InIndex)
{
	if (bLoading || !Graph) return;
	
	Sequence.Add(InIndex);
	if (UGameSectionNode* Node = Graph->GetLeafInSequence<UGameSectionNode>(Sequence, true))
	{
		ChangeSection(Node);
	}
	else
	{
		Sequence.RemoveAt(Sequence.Num() - 1);
	}
}

void UGameSectionManager::LoadSequence(const FGameplayTag InTag)
{
	if (UToroSaveSystem* System = UToroSaveSystem::Get(this))
	{
		if (const UGameSaveObjectBase* Save = System->GetSaveObject<UGameSaveObjectBase>(InTag))
		{
			Sequence = Save->Sequence;
			if (Sequence.IsEmpty()) Sequence.Add(0);
		}
	}
}

void UGameSectionManager::ChangeSection(UGameSectionNode* NewSection)
{
	if (bLoading || !NewSection || NewSection == Section
		|| !Graph || !Graph->AllNodes.Contains(NewSection)) return;
	
	bLoading = true;
	bWaiting = false;
	bMainLevelLoaded = false;
	
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->CinematicActor = GetWorld()->GetLevelScriptActor();
		PC->SetCinematicMode(true, true, false, true, true);
	}

	ToLoad = NewSection->GetLevels();
	
	ToUnload.Empty();
	TSet<TSoftObjectPtr<UWorld>> OldLevels = Section ? Section->GetLevels() : TSet<TSoftObjectPtr<UWorld>>{};
	for (const TSoftObjectPtr<UWorld>& Level : OldLevels)
	{
		if (!Level.IsNull() && !ToLoad.Contains(Level)) ToUnload.Add(Level);
	}

	if (ToUnload.IsEmpty())
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

	Section = NewSection;
}

void UGameSectionManager::SetWidgetHidden(const bool bInHidden)
{
	if (!Widget)
	{
		if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
		{
			Widget = Manager->FindWidget<USectionLoadWidget>();
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
	if (ToUnload.IsEmpty())
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
	
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->CinematicActor = nullptr;
		PC->SetCinematicMode(false, false, false, true, true);
	}

	bLoading = false;
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
	Graph = UToroRuntimeSettings::Get()->SectionGraph.LoadSynchronous();
}
