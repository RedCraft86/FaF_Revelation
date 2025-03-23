// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenuWidget.h"
#include "EnhancedCodeFlow.h"
#include "Components/TextBlock.h"
#include "Framework/ToroMusicManager.h"
#include "Framework/ToroPlayerController.h"
#include "UserWidgets/SettingsWidgetBase.h"
#include "Engine/AssetManager.h"
#include "Components/Button.h"
#include "ToroSettings.h"

void UMainMenuWidgetBase::OnPlayClicked()
{
	PlayAnimation(PlayGameAnim);
	Player->FadeToBlack(3.0f, false);
	UAssetManager::GetStreamableManager().RequestAsyncLoad(GameplayLevel.ToSoftObjectPath());
	FFlow::Delay(this, 3.0f, [this]
	{	
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, GameplayLevel);
	});

	if (AToroMusicManager* MusicManager = AToroMusicManager::Get(this))
	{
		MusicManager->SetThemeMuted(true, 2.0f);
	}
}

void UMainMenuWidgetBase::OnSettingsClicked()
{
	if (Settings)
	{
		Settings->ActivateWidget();
		if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
		{
			Controller->SetGameInputMode(EGameInputMode::GameAndUI, true,
				EMouseLockMode::LockAlways, false, Settings);
		}
		PlayAnimationReverse(ActivateAnim);
	}
}

void UMainMenuWidgetBase::OnExtrasClicked()
{
	PlayAnimation(ActivateAnim);
	Player->FadeToBlack(1.0f, true);
	FFlow::Delay(this, 1.0f, [this]
	{	
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, ExtrasLevel);
	});
}

void UMainMenuWidgetBase::OnQuitClicked()
{
	PlayAnimation(ActivateAnim);
	Player->FadeToBlack(1.0f, true);
	FFlow::Delay(this, 1.0f, [this]
	{	
		UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
	});
}

void UMainMenuWidgetBase::NativeConstruct()
{
	Super::Initialize();
	Player = AToroPlayerCharacter::Get(this);
	if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
	{
		FIntPoint Size;
		Controller->GetViewportSize(Size.X, Size.Y);
		Controller->SetMouseLocation(Size.X / 2, Size.Y / 2);
	}
	Return_Implementation();

	Settings = Cast<USettingsWidgetBase>(UToroWidget::CreateToroWidget(GetOwningPlayer(), SettingsWidgetClass));
	if (Settings) Settings->ParentUI = this;

	GameVersionText->SetText(UToroSettings::Get()->GetVersionLabel());
	PlayButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidgetBase::OnPlayClicked);
	SettingsButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidgetBase::OnSettingsClicked);
	QuitButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidgetBase::OnQuitClicked);
	ExtrasButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidgetBase::OnExtrasClicked);
}

void UMainMenuWidgetBase::Return_Implementation()
{
	PlayAnimation(ActivateAnim);
	if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
	{
		Controller->SetGameInputMode(EGameInputMode::GameAndUI, true,
			EMouseLockMode::LockAlways, false, this);
	}
}