// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenuWidget.h"
#include "EnhancedCodeFlow.h"
#include "Components/Button.h"
#include "UserWidgets/SettingsWidget.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroWidgetManager.h"
#include "ExtrasWidget.h"
#include "PhaseWidget.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 96;
	bAutoActivate = false;
}

void UMainMenuWidget::PlayGame()
{
	FadeOut([this]()
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, UToroUtilSettings::Get()->GameplayMap);
	});
}

void UMainMenuWidget::PhaseMenu()
{
	if (Phases)
	{
		DeactivateWidget();
		Phases->ActivateWidget();
	}
}

void UMainMenuWidget::SettingsMenu()
{
	if (Settings)
	{
		DeactivateWidget();
		Settings->ActivateWidget();
	}
}

void UMainMenuWidget::ExtrasMenu()
{
	if (Extras)
	{
		DeactivateWidget();
		Extras->ActivateWidget();
	}
}

void UMainMenuWidget::QuitGame()
{
	FadeOut([this]()
	{
		UKismetSystemLibrary::QuitGame(this, nullptr,
			EQuitPreference::Quit, false);
	});
}

void UMainMenuWidget::FadeOut(const TFunction<void()>& Callback)
{
	PlayAnimationReverse(ActivateAnim);
	if (Controller->PlayerCameraManager)
	{
		Controller->PlayerCameraManager->StartCameraFade(0.0f, 1.0f,
			1.0f, FLinearColor::Black, true, true);
	}

	FFlow::Delay(this, 1.0f, Callback);
}

void UMainMenuWidget::InitWidget()
{
	Super::InitWidget();
	Controller = AToroPlayerController::Get(this);
	PlayBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayGame);
	PhaseBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::PhaseMenu);
	SettingsBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::SettingsMenu);
	ExtrasBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::ExtrasMenu);
	QuitBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
}

void UMainMenuWidget::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		Phases = Manager->FindWidget<UPhaseWidget>();
		Extras = Manager->FindWidget<UExtrasWidget>();
		Settings = Manager->FindWidget<USettingsWidget>();
	}
}

void UMainMenuWidget::ReturnToWidget_Implementation(UUserWidget* FromWidget)
{
	if (FromWidget == Phases || FromWidget == Extras || FromWidget == Settings) ActivateWidget();
}
