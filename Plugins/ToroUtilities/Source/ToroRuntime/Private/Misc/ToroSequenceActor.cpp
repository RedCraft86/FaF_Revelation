// Copyright (C) RedCraft86. All Rights Reserved.

#include "Misc/ToroSequenceActor.h"
#include "Characters/ToroPlayerBase.h"
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
	if (!bLockPlayer) return;
	if (AToroPlayerBase* Player = AToroPlayerBase::Get(this))
	{
		Player->EnterCinematic(this);
	}
}

void AToroSequenceActor::UnlockPlayer() const
{
	if (!bLockPlayer) return;
	if (AToroPlayerBase* Player = AToroPlayerBase::Get(this))
	{
		Player->ExitCinematic();
	}
}
