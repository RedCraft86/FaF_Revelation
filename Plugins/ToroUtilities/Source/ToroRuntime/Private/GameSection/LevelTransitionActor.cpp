// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameSection/LevelTransitionActor.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroWidgetManager.h"
#include "Misc/TeleportTargetActor.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequencePlayer.h"
#include "ToroShortcutLibrary.h"
#if WITH_EDITORONLY_DATA
#include "Components/BillboardComponent.h"
#endif

ALevelTransitionActor::ALevelTransitionActor() : WidgetDelay(0.1f), bWaiting(false), bMainLevelLoaded(false)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
#if WITH_EDITORONLY_DATA
	DebugBillboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("DebugBillboard"));
	if (DebugBillboard)
	{
		DebugBillboard->bIsScreenSizeScaled = true;
		DebugBillboard->SetupAttachment(SceneRoot);
		DebugBillboard->SetWorldScale3D(FVector{0.5f});
		DebugBillboard->SetSprite(LoadObject<UTexture2D>(nullptr,
			TEXT("Texture2D'/ToroUtilities/Icons/LevelTransition.LevelTransition'")));
	}
#endif
}

TSet<TSoftObjectPtr<UWorld>> ALevelTransitionActor::BeginTransition(const TSet<TSoftObjectPtr<UWorld>>& OldLevels)
{
	bWaiting = false;
	bMainLevelLoaded = false;
	
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->CinematicActor = this;
		PC->SetCinematicMode(true, true, false, true, true);
	}
	
	ToUnload.Empty();
	if (OldLevels.IsEmpty())
	{
		ToUnload.Reserve(OldLevels.Num());
		for (const TSoftObjectPtr<UWorld>& Level : OldLevels)
		{
			if (!Level.IsNull() && !ToLoad.Contains(Level)) ToUnload.Add(Level);
		}
	}

	if (!ToUnload.IsEmpty())
	{
		if (StartSequence) StartSequence->Play();
	}
	else
	{
		SetWidgetHidden(false);
		UToroShortcutLibrary::SetCameraFade(this, 1.0f, FLinearColor::Black, true);
	}
	
	for (const TSoftObjectPtr<UWorld>& Level : ToLoad)
	{
		LoadLevel(Level);
	}

	return ToLoad;
}

void ALevelTransitionActor::SetWidgetHidden(const bool bInHidden)
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

void ALevelTransitionActor::UnloadLevel(const TSoftObjectPtr<UWorld>& InLevel) const
{
	if (InLevel.IsNull()) return;
	FLatentActionInfo Info;
	Info.Linkage = 0;
	Info.UUID = GetLatentID();
	Info.CallbackTarget = nullptr;
	Info.ExecutionFunction = NAME_None;
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, InLevel, Info , false);
}

void ALevelTransitionActor::LoadLevel(const TSoftObjectPtr<UWorld>& InLevel)
{
	if (InLevel.IsNull()) return;
	const FName MapName(*FPackageName::ObjectPathToPackageName(InLevel.ToString()));
	const bool bShouldBeVisible = InLevel == MainLevel || Levels.FindRef(InLevel);
	ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(this, MapName);
	if (Level && Level->IsLevelLoaded())
	{
		Level->SetShouldBeVisible(bShouldBeVisible);
		if (InLevel == MainLevel) OnMainLevelLoaded();
		return;
	}
	
	FLatentActionInfo Info;
	Info.Linkage = 0;
	Info.UUID = GetLatentID();
	Info.CallbackTarget = InLevel == MainLevel ? this : nullptr;
	Info.ExecutionFunction = InLevel == MainLevel ? FName(TEXT("OnMainLevelLoaded")) : NAME_None;
	UGameplayStatics::LoadStreamLevel(this, MapName, bShouldBeVisible, false, Info);
}

void ALevelTransitionActor::OnMainLevelLoaded()
{
	bMainLevelLoaded = true;
	if (ToUnload.IsEmpty())
	{
		Teleport->TeleportPlayer();
		UToroShortcutLibrary::StartCameraFade(this, 1.0f, 0.0f, 2.0f);
		OnEndSequenceFinished();
	}
	else if (bWaiting)
	{
		if (WaitSequence) WaitSequence->Stop();
		if (EndSequence) EndSequence->Play();
		else OnEndSequenceFinished();
	}
}

void ALevelTransitionActor::OnStartSequenceFinished()
{
	if (bMainLevelLoaded)
	{
		Teleport->TeleportPlayer();
		if (EndSequence) EndSequence->Play();
		else OnEndSequenceFinished();
	}
	else
	{
		bWaiting = true;
		if (WaitSequence) WaitSequence->Play();
		GetWorldTimerManager().SetTimer(WidgetTimer, this, &ThisClass::DelayedShowWidget,
			FMath::Max(0.05f, WidgetDelay), false);
	}
}

void ALevelTransitionActor::OnEndSequenceFinished()
{
	GetWorldTimerManager().ClearTimer(WidgetTimer);
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
}

void ALevelTransitionActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (StartSequence)
	{
		StartSequence->GetSequencePlayer()->OnNativeFinished.BindUObject(this, &ThisClass::OnStartSequenceFinished);
	}
	
	if (EndSequence)
	{
		EndSequence->GetSequencePlayer()->OnNativeFinished.BindUObject(this, &ThisClass::OnEndSequenceFinished);
	}

	TArray<TSoftObjectPtr<UWorld>> AllLevels;
	Levels.GenerateKeyArray(AllLevels);
	AllLevels.Insert(MainLevel, 0);
	AllLevels.RemoveAll([](const TSoftObjectPtr<UWorld>& Element)
	{
		return Element.IsNull();
	});
	
	ToLoad = TSet(AllLevels);
}

#if WITH_EDITOR
void ALevelTransitionActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!MainLevel.IsNull()) Levels.Remove(MainLevel);
}
#endif