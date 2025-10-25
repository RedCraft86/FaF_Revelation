// Copyright (C) RedCraft86. All Rights Reserved.

#include "QTEManager.h"

UQTEManager::UQTEManager()
{
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
}

const UQTEInstance* UQTEManager::InitiateEvent(const TSubclassOf<UQTEInstance> Class)
{
	if (!Class || IsValid(ActiveQTE))
	{
		return nullptr;
	}

	if (ActiveQTE = NewObject<UQTEInstance>(this, Class); ActiveQTE)
	{
		ActiveQTE->OnFinished.BindUObject(this, &UQTEManager::QuicktimeFinished);
		ActiveQTE->BeginQTE(GetWorld());

		UToroWidgetBase* BaseWidget = WidgetManager->FindOrAddWidget(ActiveQTE->WidgetClass);
		if (UQTEWidget* Widget = BaseWidget ? Cast<UQTEWidget>(BaseWidget) : nullptr)
		{
			Widget->ShowWidget(this);
		}
	}

	return ActiveQTE;
}

void UQTEManager::QuicktimeFinished(const bool bSuccess)
{
	if (ActiveQTE)
	{
		UToroWidgetBase* BaseWidget = WidgetManager->FindWidget(ActiveQTE->WidgetClass);
		if (UQTEWidget* Widget = BaseWidget ? Cast<UQTEWidget>(BaseWidget) : nullptr)
		{
			Widget->PopWidget();
		}

		TSubclassOf<UQTEInstance> Class = ActiveQTE->GetClass();
		OnQuicktimeFinished.Broadcast(bSuccess, Class);
		OnQuicktimeFinishedBP.Broadcast(bSuccess, Class);

		ActiveQTE->MarkAsGarbage();
		ActiveQTE = nullptr;
	}
}

void UQTEManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		WidgetManager = AToroWidgetManager::Get(this);
	});
}

void UQTEManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (ActiveQTE) ActiveQTE->TickQTE(DeltaTime);
}
