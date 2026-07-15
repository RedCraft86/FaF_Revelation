// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "InteractionWidget.h"
#include "InteractionManager.h"
#include "Components/TextBlock.h"

UInteractionWidget::UInteractionWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UInteractionWidget::OnTargetChanged(const TWeakObjectPtr<AActor>& Target) const
{
	FText Label = FText::GetEmpty();
	if (IInteractable::GetInteractionInfo(Target.Get(), Label))
	{
		ValidTarget->SetVisibility(ESlateVisibility::HitTestInvisible);
		InvalidTarget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		ValidTarget->SetVisibility(ESlateVisibility::Collapsed);
		InvalidTarget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	TargetLabel->SetText(Label);
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (UInteractionManager* Manager = UInteractionManager::Get(this))
	{
		Manager->OnTargetChanged.AddUObject(this, &UInteractionWidget::OnTargetChanged);
	}
}

void UInteractionWidget::NativeDestruct()
{
	if (UInteractionManager* Manager = UInteractionManager::Get(this))
	{
		Manager->OnTargetChanged.RemoveAll(this);
	}
	Super::NativeDestruct();
}
