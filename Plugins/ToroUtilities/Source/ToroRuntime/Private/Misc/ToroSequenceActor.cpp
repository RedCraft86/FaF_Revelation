// Copyright (C) RedCraft86. All Rights Reserved.

#include "Misc/ToroSequenceActor.h"
#include "Framework/ToroPlayerController.h"
#include "LevelSequencePlayer.h"

AToroSequenceActor::AToroSequenceActor(const FObjectInitializer& Init)
	: Super(Init), bLockPlayer(false)
{
}

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

void AToroSequenceActor::Stop() const
{
	GetSequencePlayer()->Stop();
	UnlockPlayer();
}

void AToroSequenceActor::SkipToEnd() const
{
	GetSequencePlayer()->GoToEndAndStop();
	UnlockPlayer();
}

void AToroSequenceActor::StopAtCurrentTime() const
{
	GetSequencePlayer()->StopAtCurrentTime();
	UnlockPlayer();
}

void AToroSequenceActor::SetPlayRate(const float InRate) const
{
	GetSequencePlayer()->SetPlayRate(InRate);
}

void AToroSequenceActor::SetPlaybackPosition(const FMovieSceneSequencePlaybackParams InParams) const
{
	GetSequencePlayer()->SetPlaybackPosition(InParams);
}

void AToroSequenceActor::LockPlayer()
{
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->CinematicActor = this;
		if (bLockPlayer) PC->SetCinematicMode(true, true, false, true, true);
	}
}

void AToroSequenceActor::UnlockPlayer() const
{
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->CinematicActor = nullptr;
		if (bLockPlayer) PC->SetCinematicMode(false, false, false, true, true);
	}
}
