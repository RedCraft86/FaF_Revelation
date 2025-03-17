// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroWidgetManager.h"
#include "ToroSettings.h"
#include "ToroWidget.h"

AToroWidgetManager::AToroWidgetManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

EToroValidPins AToroWidgetManager::GetToroWidgetManager(AToroWidgetManager*& OutObject,
	const UObject* ContextObject, const TSubclassOf<AToroWidgetManager>& Class, const int32 PlayerIndex)
{
	AToroWidgetManager* Obj = Get<AToroWidgetManager>(ContextObject, PlayerIndex);
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

UUserWidget* AToroWidgetManager::FindOrAddWidget(const TSubclassOf<UUserWidget> Class)
{
	if (!Class) return nullptr;
	UUserWidget* Obj = FindWidget(Class);
	if (!Obj)
	{
		if (Class->IsChildOf(UToroWidget::StaticClass()))
		{
			Obj = UToroWidget::CreateToroWidget(GetPlayerController(), TSubclassOf<UToroWidget>(*Class));
		}
		else
		{
			Obj = CreateWidget(GetPlayerController(), Class);
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
	const TSet<TSoftClassPtr<UUserWidget>>& StartupWidgets = UToroSettings::Get()->DefaultWidgets;
	for (const TSoftClassPtr<UUserWidget>& Widget : StartupWidgets)
	{
		FindOrAddWidget(Widget.LoadSynchronous());
	}
}
