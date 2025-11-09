// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameOverWidget.h"
#include "EnhancedCodeFlow.h"
#include "Framework/ToroPlayerController.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "UserInterface/NativeContainers.h"

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bIsRightSide(false)
{
	ContainerClass = UMenuWidgetContainer::StaticClass();
}

void UGameOverWidget::ShowWidget(const FText& DisplayName, const FText& Description, const bool bRightSide)
{
	bIsRightSide = bRightSide;
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
	OnPickSide(bIsRightSide);
	LabelText->SetText(NameText);
	ContentText->SetText(DescText);
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->SetInputConfig({EGameInputMode::UI_Only, true,
			EMouseLockMode::LockAlways, false});
	}
}

void UGameOverWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	RetryButton->OnClicked.AddUniqueDynamic(this, &UGameOverWidget::OnRetry);
}
