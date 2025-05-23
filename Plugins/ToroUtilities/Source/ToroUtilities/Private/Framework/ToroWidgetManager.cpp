// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroWidgetManager.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "ToroSettings.h"

AToroWidgetManager::AToroWidgetManager()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

UUserWidget* AToroWidgetManager::FindOrAddWidget(const TSubclassOf<UUserWidget> Class)
{
	if (!Class) return nullptr;
	UUserWidget* Obj = FindWidget(Class);
	if (!Obj)
	{
		if (Class->IsChildOf(UToroWidgetBase::StaticClass()))
		{
			Obj = UToroWidgetBase::CreateToroWidget(GetOwningPlayerController(),
				TSubclassOf<UToroWidgetBase>(*Class));
		}
		else
		{
			Obj = CreateWidget(GetOwningPlayerController(), Class);
		}
		if (Obj) Widgets.Add(Obj);
	}
	return Obj;
}

UUserWidget* AToroWidgetManager::FindWidget(const TSubclassOf<UUserWidget> Class)
{
	if (!Class) return nullptr;
	Widgets.Remove(nullptr);
	for (auto It = Widgets.CreateIterator(); It; ++It)
	{
		if (const TObjectPtr<UUserWidget>& Obj = *It; !Obj)
		{
			It.RemoveCurrent();
		}
		else if (Obj->IsA(Class))
		{
			return Obj;
		}
	}
	for (const TObjectPtr<UUserWidget>& Obj : Widgets)
	{
		if (Obj && Obj->IsA(Class)) return Obj;
	}
	return nullptr;
}

void AToroWidgetManager::BeginPlay()
{
	Super::BeginPlay();
	const UToroSettings* Settings = UToroSettings::Get();
	// for (const TSoftClassPtr<UUserWidget>& Widget : Settings->DefaultWidgets)
	// {
	// 	FindOrAddWidget(Widget.LoadSynchronous());
	// }
}
