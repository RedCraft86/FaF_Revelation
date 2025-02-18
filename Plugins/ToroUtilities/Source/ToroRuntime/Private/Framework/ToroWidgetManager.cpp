// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroWidgetManager.h"
#include "ToroRuntimeSettings.h"

AToroWidgetManager::AToroWidgetManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

EToroValidPins AToroWidgetManager::GetToroWidgetManager(AToroWidgetManager*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroWidgetManager>& Class, const int32 PlayerIndex)
{
	AToroWidgetManager* Obj = Cast<AToroWidgetManager>(UGameplayStatics::GetPlayerState(WorldContextObject, PlayerIndex));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

UToroWidget* AToroWidgetManager::FindOrAddWidget(const TSubclassOf<UToroWidget> Class)
{
	if (!Class) return nullptr;
	UToroWidget* Obj = FindWidget(Class);
	if (Obj) return Obj;
	
	Obj = UToroWidget::CreateNew(PlayerController, Class);
	if (Obj) WidgetObjs.Add(Obj);
	return Obj;
}

// ReSharper disable once CppPassValueParameterByConstReference
UToroWidget* AToroWidgetManager::FindWidget(const TSubclassOf<UToroWidget> Class)
{
	if (!Class) return nullptr;
	WidgetObjs.Remove(nullptr);
	for (const TObjectPtr<UToroWidget>& Obj : WidgetObjs)
	{
		if (Obj && Obj->IsA(Class)) return Obj;
	}

	return nullptr;
}

void AToroWidgetManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<AToroPlayerController>(GetPlayerController());
	const TSet<TSoftClassPtr<UToroWidget>>& Widgets = UToroRuntimeSettings::Get()->DefaultWidgets;
	for (const TSoftClassPtr<UToroWidget>& Widget : Widgets)
	{
		FindOrAddWidget(Widget.LoadSynchronous());
	}
}
