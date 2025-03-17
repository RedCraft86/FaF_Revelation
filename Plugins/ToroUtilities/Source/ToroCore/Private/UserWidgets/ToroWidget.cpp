// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroWidget.h"

UToroWidget::UToroWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	, ZOrder(0), HideCheckInterval(0.5f), bHidden(false), bWantHidden(false), bBeforeHidden(false), AutoHideTimer(0.5f)
{
	bAutoActivate = true;
}

UToroWidget* UToroWidget::CreateToroWidget(APlayerController* Controller, const TSubclassOf<UToroWidget>& Class)
{
	if (!Controller || !Class) return nullptr;
	if (UToroWidget* Widget = CreateWidget<UToroWidget>(Controller, Class))
	{
		Widget->GameMode = Controller->GetWorld()->GetAuthGameMode();
		if (Widget->CanCreateWidget())
		{
			Widget->InitWidget();
			Widget->AddToViewport(Widget->ZOrder);
		}
	}

	return nullptr;
}

void UToroWidget::SetHidden(const bool bInHidden)
{
	if (bWantHidden != bInHidden)
	{
		bWantHidden = bInHidden;
		AutoHideTimer = HideCheckInterval;
		UpdateHiddenState();
	}
}

bool UToroWidget::ShouldBeHidden()
{
	return bWantHidden || GetOwningPlayer()->bCinematicMode;
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
	if (AutoHideTimer <= 0.0f) UpdateHiddenState();
	else AutoHideTimer -= InDeltaTime;
}

void UToroWidget::NativeConstruct()
{
	InitAnim(ActivateAnim);
	Super::NativeConstruct();
}
