// Copyright (C) RedCraft86. All Rights Reserved.

#include "QTEManager.h"
#include "Framework/ToroPlayerController.h"

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
		else
		{
			QuicktimeFinished(true);
		}
	}

	return ActiveQTE;
}

const UQTEInstance* UQTEManager::GetEventInstance(const TSubclassOf<UQTEInstance> Class)
{
	return Class && ActiveQTE->IsA(Class) ? ActiveQTE : nullptr;
}

void UQTEManager::OnKeyPress(const FKey& Key) const
{
	if (ActiveQTE) ActiveQTE->KeyPressed(Key);
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

		ActiveQTE->UnbindDelegates();
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
		if (AToroPlayerController* PC = AToroPlayerController::Get(this))
		{
			PC->OnAnyKeyPressed.AddUObject(this, &UQTEManager::OnKeyPress);
		}
	});
}

void UQTEManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (ActiveQTE) ActiveQTE->TickQTE(DeltaTime);
}
