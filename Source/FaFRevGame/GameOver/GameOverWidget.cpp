// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameOverWidget.h"
#include "EnhancedCodeFlow.h"
#include "Framework/ToroPlayerController.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "UserInterface/NativeContainers.h"

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ContainerClass = UMenuWidgetContainer::StaticClass();
}

void UGameOverWidget::ShowWidget(const FText& DisplayName, const FText& Description)
{
	NameText = DisplayName;
	DescText = Description;
	PushWidget();
}

void UGameOverWidget::OnRetry()
{
	SetHidden(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	UToroShortcutLibrary::StartCameraFade(this, 0.0f, 1.0f, 0.5f);
	FFlow::Delay(this, 0.6f, [this]()
	{
		UToroShortcutLibrary::RestartLevel(this);
	});
}

void UGameOverWidget::PushWidget()
{
	Super::PushWidget();
	LabelText->SetText(NameText);
	ContentText->SetText(DescText);
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		PC->SetInputConfig({EGameInputMode::UI_Only, true,
			EMouseLockMode::LockAlways, false, this});
	}
}

void UGameOverWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	RetryButton->OnClicked.AddUniqueDynamic(this, &UGameOverWidget::OnRetry);
}
