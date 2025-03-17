// Copyright (C) RedCraft86. All Rights Reserved.

#include "SequenceActor.h"

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

void AToroSequenceActor::SetPlaybackPosition(FMovieSceneSequencePlaybackParams InParams) const
{
	GetSequencePlayer()->SetPlaybackPosition(InParams);
}

void AToroSequenceActor::LockPlayer()
{
	if (!bLockPlayer) return;
}

void AToroSequenceActor::UnlockPlayer() const
{
	if (!bLockPlayer) return;
	// TODO
}

void AToroSequenceActor::BeginPlay()
{
	Super::BeginPlay();
	OnSequenceFinished().BindUObject(this, &AToroSequenceActor::UnlockPlayer);
}
