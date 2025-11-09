// Copyright (C) RedCraft86. All Rights Reserved.

#include "QTEManager.h"
#include "Framework/ToroPlayerController.h"
#include "Helpers/LoggingMacros.h"

UQTEManager::UQTEManager()
{
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
}

void UQTEManager::InitiateEvent(UQTEInstance* InObject)
{
	if (InObject && ActiveQTE && InObject != ActiveQTE)
	{
		InObject->OnFinished.BindUObject(this, &UQTEManager::QuicktimeFinished);
		InObject->BeginQTE(GetWorld());
		ActiveQTE = InObject;

		SetComponentTickEnabled(true);

		UToroWidgetBase* BaseWidget = WidgetManager->FindOrAddWidget(ActiveQTE->WidgetClass);
		if (UQTEWidget* Widget = BaseWidget ? Cast<UQTEWidget>(BaseWidget) : nullptr)
		{
			Widget->ShowWidget(this);
		}
		else
		{
			QuicktimeFinished(true);
			UE_KLOG_ERROR(2.0f, TEXT("QTE %s is missing widget!"),
				*GetNameSafe(InObject ? InObject->GetClass() : nullptr))
		}
	}
}

const UQTEInstance* UQTEManager::GetEventInstance(const TSubclassOf<UQTEInstance> Class)
{
	return Class && ActiveQTE->IsA(Class) ? ActiveQTE : nullptr;
}

void UQTEManager::OnKeyPress(const FKey& Key) const
{
	if (ActiveQTE && !ActiveQTE->IsFinished())
	{
		ActiveQTE->KeyPressed(Key);
	}
}

void UQTEManager::QuicktimeFinished(const bool bSuccess)
{
	SetComponentTickEnabled(false);
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
	if (ActiveQTE && !ActiveQTE->IsFinished())
	{
		ActiveQTE->TickQTE(DeltaTime);
	}
}
