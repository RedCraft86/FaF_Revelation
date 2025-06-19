// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/GamePauseWidget.h"
#include "UserWidgets/GameSettingsWidget.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroWidgetManager.h"
#include "Libraries/ToroGeneralUtils.h"
#include "EnhancedCodeFlow.h"

UGamePauseWidget::UGamePauseWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 96;
	bAutoActivate = false;
}

void UGamePauseWidget::ResumeGame()
{
	OnGamePause(false);
}

void UGamePauseWidget::SettingsMenu()
{
	if (Settings)
	{
		DeactivateWidget();
		Settings->ActivateWidget();
	}
}

void UGamePauseWidget::Checkpoint()
{
	FadeOut([this]()
	{
		UToroGeneralUtils::RestartLevel(this);
	});
}

void UGamePauseWidget::MainMenu()
{
	FadeOut([this]()
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, UToroSettings::Get()->LaunchMap);
	});
}

void UGamePauseWidget::QuitGame()
{
	FadeOut([this]()
	{
		UKismetSystemLibrary::QuitGame(this, Controller,
			EQuitPreference::Quit, false);
	});
}

void UGamePauseWidget::OnGamePause(const bool bPaused)
{
	if (bPaused)
	{
		CachedInput = Controller->GetInputModeData();
		Controller->SetInputModeData({EGameInputMode::UI_Only, true,
			EMouseLockMode::LockAlways, true, this});

		ActivateWidget();
	}
	else
	{
		Controller->SetInputModeData(CachedInput);
		DeactivateWidget();
	}
}

void UGamePauseWidget::FadeOut(const TFunction<void()>& Callback)
{
	PlayAnimationReverse(ActivateAnim);
	if (Controller->PlayerCameraManager)
	{
		Controller->PlayerCameraManager->StartCameraFade(0.0f, 1.0f,
			1.0f, FLinearColor::Black, true, true);
	}

	FFlow::Delay(this, 1.0f, Callback);
}

void UGamePauseWidget::InitWidget()
{
	Super::InitWidget();
	Controller = AToroPlayerController::Get(this);
	Controller->OnGamePaused.AddUObject(this, &UGamePauseWidget::OnGamePause);
}

void UGamePauseWidget::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		Settings = Manager->FindWidget<UGameSettingsWidget>();
	}
}

void UGamePauseWidget::ReturnToWidget_Implementation(UUserWidget* FromWidget)
{
	if (FromWidget == Settings) ActivateWidget();
}
