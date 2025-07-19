// Copyright (C) RedCraft86. All Rights Reserved.

#include "MiscActors/ToroSequenceActor.h"
#include "UserWidgets/CutsceneSkipWidget.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroWidgetManager.h"
#include "SaveSystem/ToroSaveManager.h"
#include "SaveSystem/ToroGlobalSave.h"
#include "LevelSequencePlayer.h"

void AToroSequenceActor::Play()
{
	GetSequencePlayer()->Play();
	LockPlayer();
}

void AToroSequenceActor::Reverse()
{
	GetSequencePlayer()->PlayReverse();
	LockPlayer();
}

void AToroSequenceActor::Stop()
{
	GetSequencePlayer()->Stop();
	UnlockPlayer();
}

void AToroSequenceActor::SkipToEnd()
{
	GetSequencePlayer()->GoToEndAndStop();
	UnlockPlayer();
}

void AToroSequenceActor::StopAtCurrentTime()
{
	GetSequencePlayer()->StopAtCurrentTime();
	UnlockPlayer();
}

void AToroSequenceActor::SetPlayRate(const float InRate) const
{
	GetSequencePlayer()->SetPlayRate(InRate);
}

void AToroSequenceActor::SetPlaybackPosition(FMovieSceneSequencePlaybackParams InParams) const
{
	GetSequencePlayer()->SetPlaybackPosition(InParams);
}

void AToroSequenceActor::OnFinished()
{
	UnlockPlayer();
	OnFinishedEvent.Broadcast();

	if (!bSkippable) return;
	if (UToroGlobalSave* Save = UToroSaveManager::GetSaveObject<UToroGlobalSave>(this, SaveTags::TAG_Global))
	{
		Save->Cutscenes.Add(CutsceneGuid);
	}
}

void AToroSequenceActor::LockPlayer()
{
	if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
	{
		if (bLockPlayer) Controller->EnterCinematic(this);

		if (!bSkippable) return;
		if (const UToroGlobalSave* Save = UToroSaveManager::GetSaveObject<UToroGlobalSave>(this, SaveTags::TAG_Global))
		{
			if (!Save->Cutscenes.Contains(CutsceneGuid)) return;
		}
		if (UCutsceneSkipWidget* SkipWidget = GetWidget())
		{
			SkipWidget->ShowWidget(this);
			CachedInput = Controller->GetInputModeData();
			Controller->SetInputModeData({EGameInputMode::GameAndUI, false,
				EMouseLockMode::LockAlways, true, SkipWidget});
		}
	}
}

void AToroSequenceActor::UnlockPlayer()
{
	if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
	{
		if (bLockPlayer) Controller->ExitCinematic();

		if (!bSkippable) return;
		if (UCutsceneSkipWidget* SkipWidget = GetWidget(); SkipWidget && SkipWidget->IsActivated())
		{
			SkipWidget->DeactivateWidget();
			Controller->SetInputModeData(CachedInput);
		}
	}
}

UCutsceneSkipWidget* AToroSequenceActor::GetWidget()
{
	if (Widget) return Widget;
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		Widget = Manager->FindWidget<UCutsceneSkipWidget>();
	}
	return Widget;
}

void AToroSequenceActor::BeginPlay()
{
	Super::BeginPlay();
	GetSequencePlayer()->OnNativeFinished.BindUObject(this, &AToroSequenceActor::OnFinished);
}

void AToroSequenceActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!CutsceneGuid.IsValid()) { CutsceneGuid = FGuid::NewGuid(); }
}
