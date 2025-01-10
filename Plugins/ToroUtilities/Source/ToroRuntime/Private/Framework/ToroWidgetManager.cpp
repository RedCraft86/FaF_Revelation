// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroWidgetManager.h"

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

UToroUserWidget* AToroWidgetManager::FindOrAddWidget(const TSubclassOf<UToroUserWidget>& Class)
{
	if (!Class) return nullptr;
	UToroUserWidget* Widget = FindWidget(Class);
	if (!Widget)
	{
		Widget = UToroUserWidget::CreateSmartWidget(GetPlayerController(), Class);
		WidgetObjs.Add(Class, Widget);
	}
	return Widget;
}

UToroUserWidget* AToroWidgetManager::FindWidget(const TSubclassOf<UToroUserWidget>& Class)
{
	if (!Class) return nullptr;
	for (const TPair<TSubclassOf<UToroUserWidget>, TObjectPtr<UToroUserWidget>>& Pair : WidgetObjs)
	{
		if (Pair.Value && Pair.Value->IsA(Class))
		{
			return Pair.Value;
		}
	}
	return nullptr;
}

void AToroWidgetManager::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = GetPlayerController();
	for (const TSubclassOf<UToroUserWidget>& Class : DefaultWidgets)
	{
		UToroUserWidget* Widget = UToroUserWidget::CreateSmartWidget(PlayerController, Class);
		WidgetObjs.Add(Class, Widget);
	}
}
