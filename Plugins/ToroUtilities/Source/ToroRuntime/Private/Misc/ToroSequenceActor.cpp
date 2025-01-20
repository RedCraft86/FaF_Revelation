// Copyright (C) RedCraft86. All Rights Reserved.

#include "Misc/ToroSequenceActor.h"
#include "Framework/ToroPlayerController.h"
#include "LevelSequencePlayer.h"

void AToroSequenceActor::Play()
{
	GetSequencePlayer()->Play();
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->CinematicActor = this;
		PC->SetCinematicMode(true, true, false, true, true);
	}
}

void AToroSequenceActor::Reverse()
{
	GetSequencePlayer()->PlayReverse();
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->CinematicActor = this;
		PC->SetCinematicMode(true, true, false, true, true);
	}
}

void AToroSequenceActor::Stop() const
{
	GetSequencePlayer()->Stop();
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->CinematicActor = nullptr;
		PC->SetCinematicMode(false, true, false, true, true);
	}
}

void AToroSequenceActor::SkipToEnd() const
{
	GetSequencePlayer()->GoToEndAndStop();
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->CinematicActor = nullptr;
		PC->SetCinematicMode(false, true, false, true, true);
	}
}

void AToroSequenceActor::StopAtCurrentTime() const
{
	GetSequencePlayer()->StopAtCurrentTime();
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->CinematicActor = nullptr;
		PC->SetCinematicMode(false, true, false, true, true);
	}
}

void AToroSequenceActor::SetPlayRate(const float InRate) const
{
	GetSequencePlayer()->SetPlayRate(InRate);
}

void AToroSequenceActor::SetPlaybackPosition(const FMovieSceneSequencePlaybackParams InParams) const
{
	GetSequencePlayer()->SetPlaybackPosition(InParams);
}
