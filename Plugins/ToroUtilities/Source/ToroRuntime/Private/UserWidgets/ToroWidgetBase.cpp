// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/ToroWidgetBase.h"

#define HIDE_CHECK_INTERVAL 0.5f

UToroWidget::UToroWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), ZOrder(0)
	, bHidden(false), bWantHidden(false), bBeforeHidden(false), AutoHideTimer(HIDE_CHECK_INTERVAL)
{
	bAutoActivate = true;
}

UToroWidget* UToroWidget::CreateNew(AToroPlayerController* Owner, const TSubclassOf<UToroWidget> Class)
{
	if (!Owner || !Class) return nullptr;
	if (UToroWidget* NewWidget = CreateWidget<UToroWidget>(Owner, Class))
	{
		NewWidget->InitWidget();
		NewWidget->AddToViewport(NewWidget->ZOrder);
		return NewWidget;
	}

	return nullptr;
}

void UToroWidget::SetHidden(const bool bInHidden)
{
	if (bWantHidden != bInHidden)
	{
		bWantHidden = bInHidden;
		AutoHideTimer = HIDE_CHECK_INTERVAL;
		UpdateHiddenState();
	}
}

void UToroWidget::InitWidget()
{
	GameMode = GetWorld()->GetAuthGameMode<AToroGameMode>();
}

bool UToroWidget::ShouldBeHidden()
{
	if (bWantHidden) return true;
	if (const APlayerController* PC = GetOwningPlayer())
	{
		if (PC->bCinematicMode) return true;
	}

	return false;
}

void UToroWidget::UpdateHiddenState()
{
	if (const bool bTarget = ShouldBeHidden(); bHidden != bTarget)
	{
		bHidden = bTarget;
		if (bHidden)
		{
			DeactivateWidget();
		}
		else if (bBeforeHidden)
		{
			ActivateWidget();
		}
	}
}

void UToroWidget::InternalProcessActivation()
{
	if (bHidden)
	{
		bBeforeHidden = true;
		return;
	}
	PlayAnimationForward(ActivateAnim);
	Super::InternalProcessActivation();
}

void UToroWidget::InternalProcessDeactivation()
{
	bBeforeHidden = false;
	PlayAnimationReverse(ActivateAnim);
	Super::InternalProcessDeactivation();
}

void UToroWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (AutoHideTimer > 0.0f) AutoHideTimer -= InDeltaTime;
	else
	{
		UpdateHiddenState();
	}
}
