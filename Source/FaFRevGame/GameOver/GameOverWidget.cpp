// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameOverWidget.h"
#include "Framework/ToroPlayerController.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "UserInterface/NativeContainers.h"

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bIsRightSide(false)
{
	bAutoPush = false;
	ContainerClass = UMenuWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::Visible);
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

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
	{
		UToroShortcutLibrary::RestartLevel(this);
	}, 0.6f, false);
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
