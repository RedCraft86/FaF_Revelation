// Copyright (C) RedCraft86. All Rights Reserved.

#include "AsyncPlayWidgetAnim.h"
#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"

UAsyncPlayWidgetAnim* UAsyncPlayWidgetAnim::PlayWidgetAnimation(UUserWidget* Target, UWidgetAnimation* InAnimation, const float InPlaybackSpeed, const EUMGSequencePlayMode::Type InPlayDirection)
{
	BEGIN_CREATE_TASK
	Task->Widget = Target;
	Task->Animation = InAnimation;
	Task->PlaybackSpeed = FMath::Max(InPlaybackSpeed, 0.05f);
	Task->PlayDirection = InPlayDirection;
	FINISH_CREATE_TASK
}

void UAsyncPlayWidgetAnim::Activate()
{
	BEGIN_RUNNING
	
	if (!IsValid(Widget))
	{
		UE_KLOG_WARNING(2.0f, TEXT("Failed to run Async Action %s. Target User Widget is not provided."), *GetClass()->GetName())
		FINISH_RUNNING
		return;
	}

	if (!IsValid(Animation))
	{
		UE_KLOG_WARNING(2.0f, TEXT("Failed to run Async Action %s. Animation is not provided."), *GetClass()->GetName())
		FINISH_RUNNING
		return;
	}

	if (UUMGSequencePlayer* Player = Widget->PlayAnimation(Animation, 0.0f, 1, PlayDirection, PlaybackSpeed))
	{
		Player->OnSequenceFinishedPlaying().AddUObject(this, &UAsyncPlayWidgetAnim::OnFinishPlaying);
		return;
	}

	UE_KLOG_WARNING(2.0f, TEXT("Failed to complete Async Action %s. Sequence Player was null."), *GetClass()->GetName())
	FINISH_RUNNING
}

void UAsyncPlayWidgetAnim::OnFinishPlaying(UUMGSequencePlayer& InPlayer)
{
	InPlayer.OnSequenceFinishedPlaying().RemoveAll(this);
	OnFinished.Broadcast();
	FINISH_RUNNING
}
