// Copyright (C) RedCraft86. All Rights Reserved.

#include "MiscActors/ToroCutsceneActor.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "SaveSystem/ToroNativeSaves.h"
#include "SaveSystem/ToroSaveManager.h"
#include "LevelSequencePlayer.h"

AToroCutsceneActor::AToroCutsceneActor(const FObjectInitializer& Init)
	: Super(Init), bSkippable(false)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetCanBeDamaged(false);
}

void AToroCutsceneActor::PlayAfterFade(const float FadeTime)
{
	if (!GetWorldTimerManager().IsTimerActive(FadedPlayHandle))
	{
		UToroShortcutLibrary::StartCameraFade(this, 0.0f, 1.0f, FadeTime);
		GetWorldTimerManager().SetTimer(FadedPlayHandle, [this]()
		{
			FadedPlayHandle.Invalidate();
			PlayCutscene();
		}, FadeTime + UE_SMALL_NUMBER, false);
	}
}

void AToroCutsceneActor::PlayCutscene()
{
	if (!GetWorldTimerManager().IsTimerActive(FadedPlayHandle))
	{
		UToroShortcutLibrary::ClearCameraFade(this);
		GetSequencePlayer()->Play();
		LockPlayer();
	}
}

void AToroCutsceneActor::SkipCutscene()
{
	if (!GetWorldTimerManager().IsTimerActive(FadedPlayHandle))
	{
		GetSequencePlayer()->GoToEndAndStop();
		UnlockPlayer();
	}
}

void AToroCutsceneActor::OnFinished()
{
	UnlockPlayer();
	OnFinishedEvent.Broadcast();
	if (UToroGlobalSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>() : nullptr)
	{
		Save->Cutscenes.Add(CutsceneGuid);
		Save->SaveObject(nullptr);
	}
}

void AToroCutsceneActor::LockPlayer()
{
	if (AToroPlayerCharacter* Player = AToroPlayerCharacter::Get(this))
	{
		// Controller->EnterCinematic(this); // TODO
	}

	if (!bSkippable) return;
	const UToroGlobalSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>() : nullptr;
	if (!Save || Save->Cutscenes.Contains(CutsceneGuid))
	{
		// TODO ui
	}
}

void AToroCutsceneActor::UnlockPlayer()
{
	if (AToroPlayerCharacter* Player = AToroPlayerCharacter::Get(this))
	{
		// Controller->ExitCinematic(); // TODO
	}

	if (!bSkippable) return;
	// TODO ui
}

void AToroCutsceneActor::BeginPlay()
{
	Super::BeginPlay();
	GetSequencePlayer()->OnNativeFinished.BindUObject(this, &AToroCutsceneActor::OnFinished);
}

void AToroCutsceneActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!CutsceneGuid.IsValid()) CutsceneGuid = FGuid::NewGuid();
}
