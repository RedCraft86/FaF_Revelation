// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroWidgetManager.h"
#include "Blueprint/UserWidget.h"

EToroValidPins AToroWidgetManager::GetToroWidgetManager(AToroWidgetManager*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroWidgetManager>& Class, const int32 PlayerIndex)
{
	AToroWidgetManager* Obj = Cast<AToroWidgetManager>(UGameplayStatics::GetPlayerState(WorldContextObject, PlayerIndex));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

UUserWidget* AToroWidgetManager::FindOrAddWidget(const TSubclassOf<UUserWidget>& Class)
{
	if (!Class) return nullptr;
	UUserWidget* Widget = FindWidget(Class);
	if (!Widget)
	{
		Widget = CreateWidget<UUserWidget>(GetPlayerController(), Class);
		WidgetObjs.Add(Class, Widget);
	}
	return Widget;
}

UUserWidget* AToroWidgetManager::FindWidget(const TSubclassOf<UUserWidget>& Class)
{
	if (!Class) return nullptr;
	if (const TObjectPtr<UUserWidget>* Widget = WidgetObjs.Find(Class); Widget && *Widget)
	{
		return *Widget;
	}

	return nullptr;
}

void AToroWidgetManager::BeginPlay()
{
	Super::BeginPlay();
	for (const TSubclassOf<UUserWidget>& Class : DefaultWidgets)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetPlayerController(), Class);
		WidgetObjs.Add(Class, Widget);
	}
}
