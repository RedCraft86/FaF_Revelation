// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/ToroWidgetBase.h"

#define MAX_Z_ORDER 50

UToroWidgetBase::UToroWidgetBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer), ZOrder(0)
	, HideCheckInterval(0.5f), bHidden(false), bWantHidden(false), bBeforeHidden(false), AutoHideTimer(0.5f)
{
	bAutoActivate = true;
}

UToroWidgetBase* UToroWidgetBase::CreateToroWidget(APlayerController* Controller, const TSubclassOf<UToroWidgetBase>& Class)
{
	if (!Controller || !Class) return nullptr;
	if (UToroWidgetBase* Widget = CreateWidget<UToroWidgetBase>(Controller, Class))
	{
		Widget->GameMode = Controller->GetWorld()->GetAuthGameMode();
		if (Widget->CanCreateWidget())
		{
			Widget->InitWidget();
			Widget->AddToViewport(FMath::Min(MAX_Z_ORDER, (int32)Widget->ZOrder));
			return Widget;
		}
	}

	return nullptr;
}

void UToroWidgetBase::SetHidden(const bool bInHidden)
{
	if (bWantHidden != bInHidden)
	{
		bWantHidden = bInHidden;
		AutoHideTimer = HideCheckInterval;
		UpdateHiddenState();
	}
}

bool UToroWidgetBase::ShouldBeHidden()
{
	return bWantHidden || GetOwningPlayer()->bCinematicMode;
}

void UToroWidgetBase::UpdateHiddenState()
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

void UToroWidgetBase::InternalProcessActivation()
{
	if (bHidden)
	{
		bBeforeHidden = true;
		return;
	}
	PlayAnimationForward(ActivateAnim);
	Super::InternalProcessActivation();
}

void UToroWidgetBase::InternalProcessDeactivation()
{
	bBeforeHidden = false;
	PlayAnimationReverse(ActivateAnim);
	Super::InternalProcessDeactivation();
}

void UToroWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (AutoHideTimer <= 0.0f)
	{
		UpdateHiddenState();
		AutoHideTimer = HideCheckInterval;
	}
	else AutoHideTimer -= InDeltaTime;
}

void UToroWidgetBase::NativeConstruct()
{
	InitAnim(ActivateAnim);
	Super::NativeConstruct();
}
