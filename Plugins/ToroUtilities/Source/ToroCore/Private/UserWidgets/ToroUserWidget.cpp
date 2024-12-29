// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroUserWidget.h"

UToroUserWidget::UToroUserWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer),ZOrder(0), bAutoAdd(false), bRemoving(false), bWidgetHidden(false)
{
}

UToroUserWidget* UToroUserWidget::CreateSmartWidget(APlayerController* Owner, const TSubclassOf<UToroUserWidget> Class)
{
	if (UToroUserWidget* Widget = CreateWidget<UToroUserWidget>(Owner, Class))
	{
		Widget->PlayerController = Owner;
		Widget->GameMode = Owner ? Owner->GetWorld()->GetAuthGameMode() : nullptr;
		Widget->InitWidget();
		return Widget;
	}

	return nullptr;
}

void UToroUserWidget::SetWidgetHidden(const bool bInHidden)
{
	if (!IsInViewport() || bRemoving) return;
	if (bWidgetHidden != bInHidden)
	{
		bWidgetHidden = bInHidden;
		if (IsValid(ShowHideAnim))
		{
			UUMGSequencePlayer* Player;
			if (bWidgetHidden)
			{
				Player = PlayAnimationReverse(ShowHideAnim);
			}
			else
			{
				Player = PlayAnimationForward(ShowHideAnim);
			}

			if (Player) Player->OnSequenceFinishedPlaying().Clear();
		}
		else
		{
			SetVisibility(bWidgetHidden ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UToroUserWidget::AddUserWidget(FToroWidgetAddRemoveSignature OnFinished)
{
	OnFinished.IsBound() ? AddWidget([OnFinished](){ OnFinished.Execute(); }) : AddWidget(nullptr);
}

void UToroUserWidget::RemoveUserWidget(FToroWidgetAddRemoveSignature OnFinished)
{
	OnFinished.IsBound() ? RemoveWidget([OnFinished](){ OnFinished.Execute(); }) : RemoveWidget(nullptr);
}

void UToroUserWidget::InitWidget()
{
	if (bAutoAdd) AddWidget(nullptr);
}

void UToroUserWidget::AddWidget(const TFunction<void()>& OnFinished)
{
	if (IsInViewport()) return;

	AddToViewport(ZOrder);
	if (ShowHideAnim)
	{
		if (UUMGSequencePlayer* Player = PlayAnimationForward(ShowHideAnim))
		{
			if (OnFinished)
			{
				Player->OnSequenceFinishedPlaying().Clear();
				Player->OnSequenceFinishedPlaying().AddLambda([OnFinished](UUMGSequencePlayer&)
				{
					OnFinished();
				});
			}
		}
	}
	else if (OnFinished)
	{
		OnFinished();
	}
}

void UToroUserWidget::RemoveWidget(const TFunction<void()>& OnFinished)
{
	if (!IsInViewport() || bRemoving) return;

	bRemoving = true;
	bWidgetHidden = false;
	if (ShowHideAnim)
	{
		if (UUMGSequencePlayer* Player = PlayAnimationReverse(ShowHideAnim))
		{
			Player->OnSequenceFinishedPlaying().Clear();
			Player->OnSequenceFinishedPlaying().AddLambda([this, OnFinished](UUMGSequencePlayer&)
			{
				bRemoving = false;
				RemoveFromParent();
				if (OnFinished) OnFinished();
			});
		}
	}
	else
	{
		bRemoving = false;
		RemoveFromParent();
		if (OnFinished) OnFinished();
	}
}
