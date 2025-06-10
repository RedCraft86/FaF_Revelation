// Copyright (C) RedCraft86. All Rights Reserved.

#include "MiscActors/ToroSequenceActor.h"
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

void AToroSequenceActor::OnFinished()
{
	UnlockPlayer();
	OnFinishedEvent.Broadcast();
}

void AToroSequenceActor::LockPlayer()
{
	// AToroPlayerController* Controller = AToroPlayerController::Get(this);
	// if (bLockPlayer && Controller)
	// {
	// 	Controller->EnterCinematic(this);
	// }
}

void AToroSequenceActor::UnlockPlayer() const
{
	// AToroPlayerController* Controller = AToroPlayerController::Get(this);
	// if (bLockPlayer && Controller)
	// {
	// 	Controller->ExitCinematic();
	// }
}

void AToroSequenceActor::BeginPlay()
{
	Super::BeginPlay();
	GetSequencePlayer()->OnNativeFinished.BindUObject(this, &AToroSequenceActor::OnFinished);
}
