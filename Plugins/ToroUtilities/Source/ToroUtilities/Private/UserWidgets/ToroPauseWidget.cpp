// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/ToroPauseWidget.h"
#include "UserWidgets/ToroSettingsWidget.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroWidgetManager.h"
#include "Libraries/ToroGeneralUtils.h"
#include "EnhancedCodeFlow.h"

UToroPauseWidget::UToroPauseWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 96;
	bAutoActivate = false;
}

void UToroPauseWidget::ResumeGame()
{
	OnGamePause(false);
}

void UToroPauseWidget::SettingsMenu()
{
	if (Settings)
	{
		DeactivateWidget();
		Settings->ActivateWidget();
	}
}

void UToroPauseWidget::Checkpoint()
{
	FadeOut([this]()
	{
		UToroGeneralUtils::RestartLevel(this);
	});
}

void UToroPauseWidget::MainMenu()
{
	FadeOut([this]()
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, UToroSettings::Get()->LaunchMap);
	});
}

void UToroPauseWidget::QuitGame()
{
	FadeOut([this]()
	{
		UKismetSystemLibrary::QuitGame(this, Controller,
			EQuitPreference::Quit, false);
	});
}

void UToroPauseWidget::OnGamePause(const bool bPaused)
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

void UToroPauseWidget::FadeOut(const TFunction<void()>& Callback)
{
	PlayAnimationReverse(ActivateAnim);
	if (Controller->PlayerCameraManager)
	{
		Controller->PlayerCameraManager->StartCameraFade(0.0f, 1.0f,
			1.0f, FLinearColor::Black, true, true);
	}

	FFlow::Delay(this, 1.0f, Callback);
}

void UToroPauseWidget::InitWidget()
{
	Super::InitWidget();
	Controller = AToroPlayerController::Get(this);
	Controller->OnGamePaused.AddUObject(this, &UToroPauseWidget::OnGamePause);
}

void UToroPauseWidget::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		Settings = Manager->FindWidget<UToroSettingsWidget>();
	}
}

void UToroPauseWidget::ReturnToWidget_Implementation(UUserWidget* FromWidget)
{
	if (FromWidget == Settings) ActivateWidget();
}
