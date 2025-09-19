// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserInterface/ToroWidgetBase.h"

#define FADE_CHECK_INTERVAL 0.5f

UToroWidgetBase::UToroWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bWantsToShow(true), bFadeState(true), FadeCheckTime(0.0f)
{
	bAutoActivate = true;
}

UToroWidgetBase* UToroWidgetBase::CreateToroWidget(APlayerController* Owner, const TSubclassOf<UToroWidgetBase> WidgetClass)
{
	if (!Owner || !WidgetClass)
	{
		return nullptr;
	}

	const UToroWidgetBase* CDO = WidgetClass->GetDefaultObject<UToroWidgetBase>();
	if (!CDO || !CDO->CanCreateWidget(Owner))
	{
		return nullptr;
	}

	if (UToroWidgetBase* Widget = CreateWidget<UToroWidgetBase>(Owner, WidgetClass))
	{
		Widget->InitWidget(Owner);
		return Widget;
	}

	return nullptr;
}

void UToroWidgetBase::FadeInWidget()
{
	bWantsToShow = true;
	FadeCheckTime = FADE_CHECK_INTERVAL;
}

void UToroWidgetBase::FadeOutWidget()
{
	bWantsToShow = false;
	FadeCheckTime = FADE_CHECK_INTERVAL;
}

void UToroWidgetBase::UpdateFadeState()
{
	const bool bTargetFade = bWantsToShow && ShouldShowWidget();
	if (bFadeState != bTargetFade)
	{
		bFadeState = bTargetFade;
		bFadeState ? PlayAnimationForward(FadeAnim) : PlayAnimationReverse(FadeAnim);
	}
}

void UToroWidgetBase::InitWidget(APlayerController* Controller)
{
	GameMode = Controller->GetWorld()->GetAuthGameMode();
}

void UToroWidgetBase::NativeConstruct()
{
	InitAnim(FadeAnim)
	Super::NativeConstruct();
}

void UToroWidgetBase::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	SetVisibility(ESlateVisibility::Visible);
	FadeInWidget();
}

void UToroWidgetBase::InternalProcessDeactivation()
{
	FadeOutWidget();
	SetVisibility(ESlateVisibility::HitTestInvisible);
	Super::InternalProcessDeactivation();
}

void UToroWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (FadeCheckTime < FADE_CHECK_INTERVAL)
	{
		FadeCheckTime += InDeltaTime;
		return;
	}
	FadeCheckTime = 0.0f;
	UpdateFadeState();
}
