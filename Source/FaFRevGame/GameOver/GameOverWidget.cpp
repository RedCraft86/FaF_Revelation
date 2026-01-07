// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameOverWidget.h"
#include "GameOverDatabase.h"
#include "Player/PlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "UserInterface/NativeContainers.h"
#include "UserInterface/ToroWidgetManager.h"

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bAutoPush = false;
	ContainerClass = UMenuWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::Visible);
}

void UGameOverWidget::ShowGameOver(const UObject* ContextObject)
{
	if (UGameOverWidget* Widget = AToroWidgetManager::GetWidget<UGameOverWidget>(ContextObject))
	{
		Widget->PushWidget();
	}
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

	PlayAnimation(ShowAnim);

	HintButton->SetVisibility(ESlateVisibility::Collapsed);
	if (const APlayerCharacter* Player = APlayerCharacter::Get<APlayerCharacter>(this))
	{
		const UGameOverDatabase* Database = UToroSettings::Get()->GetDatabase<UGameOverDatabase>();
		if (const FGameOverCharEntry* Entry = Database ? Database->GetEntry(Player->GetJumpscareEnemy()) : nullptr)
		{
			LabelText->SetText(Entry->Name);
			ContentText->SetText(Entry->Description);
			HintButton->SetVisibility(ESlateVisibility::Visible);
		}
	}

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
	HintButton->OnClicked.AddUniqueDynamic(this, &UGameOverWidget::OnHint);
}
