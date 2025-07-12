// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenuWidget.h"
#include "EnhancedCodeFlow.h"
#include "Components/Button.h"
#include "UserWidgets/SettingsWidget.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroWidgetManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "PhaseWidget.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 96;
	bAutoActivate = false;
}

void UMainMenuWidget::PlayGame()
{
	DeactivateWidget();
	Difficulty->ParentUI = this;
	Difficulty->ActivateWidget();

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(UToroUtilSettings::Get()->GameplayMap.ToSoftObjectPath());
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
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(UToroUtilSettings::Get()->ExtrasMap.ToSoftObjectPath());
	FadeOut([this]()
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, UToroUtilSettings::Get()->ExtrasMap);
	});
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
		Phases->ParentUI = this;
		
		Settings = Manager->FindWidget<USettingsWidget>();
		Settings->ParentUI = this;
		
		Difficulty = Manager->FindWidget<UDifficultyWidget>();
	}
}

void UMainMenuWidget::ReturnToWidget_Implementation(UUserWidget* FromWidget)
{
	if (FromWidget == Phases || FromWidget == Settings) ActivateWidget();
	else if (FromWidget == Difficulty)
	{
		FadeOut([this]()
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, UToroUtilSettings::Get()->GameplayMap);
		});
	}
}
