// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserInterface/ToroWidgetBase.h"

UToroWidgetBase::UToroWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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

void UToroWidgetBase::InitWidget(APlayerController* Controller)
{
	GameMode = Controller->GetWorld()->GetAuthGameMode();
}

void UToroWidgetBase::NativeConstruct()
{
	if (FadeAnim) InitAnim(FadeAnim)
	Super::NativeConstruct();
}

void UToroWidgetBase::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	if (FadeAnim) PlayAnimationForward(FadeAnim);
}

void UToroWidgetBase::InternalProcessDeactivation()
{
	if (FadeAnim) PlayAnimationReverse(FadeAnim);
	Super::InternalProcessDeactivation();
}
