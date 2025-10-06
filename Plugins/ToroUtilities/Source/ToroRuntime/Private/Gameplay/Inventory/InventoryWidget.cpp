// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inventory/InventoryWidget.h"
#include "Framework/ToroPlayerController.h"
#include "Inventory/InventoryManager.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
}

void UInventoryWidget::UpdateWidget()
{
	// TODO impl
}

void UInventoryWidget::PushWidget()
{
	Super::PushWidget();
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		InputConfig = PC->GetInputConfig();
		PC->SetInputConfig({EGameInputMode::GameAndUI, true,
			EMouseLockMode::LockAlways, false, this});
		PC->AddPauseRequest(this);
	}
	UpdateWidget();
}

void UInventoryWidget::PopWidget()
{
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		PC->SetInputConfig(InputConfig);
		PC->RemovePauseRequest(this);
	}
	Super::PopWidget();
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		Manager = UInventoryManager::Get(this);
	});
}
