// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserInterface/ToroWidgetManager.h"

AToroWidgetManager::AToroWidgetManager()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

UToroWidgetBase* AToroWidgetManager::AddWidget(const TSubclassOf<UToroWidgetBase> WidgetClass)
{
	if (!WidgetClass)
	{
		return nullptr;
	}

	UToroWidgetBase* Widget = FindWidget(WidgetClass);
	if (!Widget)
	{
		Widget = UToroWidgetBase::CreateToroWidget(GetOwningPlayerController(), WidgetClass);
		WidgetObjects.Add(Widget);
	}

	return Widget;
}

UToroWidgetBase* AToroWidgetManager::FindWidget(const TSubclassOf<UToroWidgetBase> WidgetClass)
{
	if (!WidgetClass)
	{
		return nullptr;
	}

	for (UToroWidgetBase* Widget : WidgetObjects)
	{
		if (Widget->IsA(WidgetClass))
		{
			return Widget;
		}
	}

	return nullptr;
}

void AToroWidgetManager::BeginPlay()
{
	Super::BeginPlay();

	// TODO automatic loading
}
