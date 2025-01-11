// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/ToroUserWidget.h"

UToroUserWidget::UToroUserWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer), ZOrder(0), bAutoAdd(false), AutoHideInterval(0.25f)
	, bRemoving(false), bHidden(false), bCachedHidden(false), HideCheck(0.0f)
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

void UToroUserWidget::SetHidden(const bool bInHidden)
{
	bHidden = bInHidden;
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
	if (FadeAnim)
	{
		if (UUMGSequencePlayer* Player = PlayAnimationForward(FadeAnim))
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
	bHidden = false;
	if (FadeAnim)
	{
		if (UUMGSequencePlayer* Player = PlayAnimationReverse(FadeAnim))
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

void UToroUserWidget::HandleAutoHiding(const bool bInHidden)
{
	if (bCachedHidden != bInHidden)
	{
		bCachedHidden = bInHidden;
		if (IsValid(AutoHideAnim))
		{
			if (bCachedHidden)
			{
				PlayAnimationReverse(AutoHideAnim);
			}
			else
			{
				PlayAnimationForward(AutoHideAnim);
			}
		}
		else
		{
			SetVisibility(bCachedHidden ? ESlateVisibility::Collapsed
				: ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UToroUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!IsInViewport() || bRemoving) return;
	HideCheck += InDeltaTime;
	if (HideCheck > AutoHideInterval)
	{
		HideCheck = 0.0f;
		HandleAutoHiding(ShouldHide());
	}
}
