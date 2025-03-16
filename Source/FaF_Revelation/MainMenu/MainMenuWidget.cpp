// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenuWidget.h"
#include "EnhancedCodeFlow.h"
#include "Components/TextBlock.h"
#include "GeneralProjectSettings.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Characters/ToroPlayerBase.h"
#include "Components/Button.h"
#include "Engine/AssetManager.h"
#include "Framework/ToroPlayerController.h"
#include "MusicSystem/ToroMusicManager.h"
#include "UserSettings/SettingsWidgetBase.h"

UMainMenuWidgetBase::UMainMenuWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMainMenuWidgetBase::OnPlayClicked()
{
	PlayAnimation(PlayGameAnim);
	PlayerChar->FadeToBlack(3.0f, false);
	UAssetManager::GetStreamableManager().RequestAsyncLoad(GameplayLevel.ToSoftObjectPath());
	FFlow::Delay(this, 3.0f, [this]
	{	
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, GameplayLevel);
	});

	if (AToroMusicManager* MusicManager = AToroMusicManager::Get(this))
	{
		MusicManager->MainThemeComponent->FadeOut(2.0f, 0.0f);
	}
}

void UMainMenuWidgetBase::OnSettingsClicked()
{
	SettingsWidget->ActivateWidget();
	if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
	{
		Controller->SetGameInputMode(EGameInputMode::GameAndUI, true,
			EMouseLockMode::LockAlways, false, SettingsWidget);
	}
	PlayAnimationReverse(ActivateAnim);
}

void UMainMenuWidgetBase::OnExtrasClicked()
{
	PlayAnimation(ActivateAnim);
	PlayerChar->FadeToBlack(1.0f, true);
	FFlow::Delay(this, 1.0f, [this]
	{	
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, ExtrasLevel);
	});
}

void UMainMenuWidgetBase::OnQuitClicked()
{
	PlayAnimation(ActivateAnim);
	PlayerChar->FadeToBlack(1.0f, true);
	FFlow::Delay(this, 1.0f, [this]
	{	
		UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
	});
}

void UMainMenuWidgetBase::NativeConstruct()
{
	Super::Initialize();
	PlayerChar = AToroPlayerBase::Get(this);

	Return_Implementation();
	if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
	{
		FIntPoint Size;
		Controller->GetViewportSize(Size.X, Size.Y);
		Controller->SetMouseLocation(Size.X / 2, Size.Y / 2);
	}

	SettingsWidget = UToroWidget::CreateNew<USettingsWidgetBase>(PlayerChar->GetPlayerController(), SettingsWidgetClass);
	if (SettingsWidget) SettingsWidget->ParentUI = this;

	if (const UGeneralProjectSettings* ProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		GameVersionText->SetText(FText::FromString(FString::Printf(TEXT("Game Version: %s | %s Build"),
			*ProjectSettings->ProjectVersion, LexToString(FApp::GetBuildConfiguration()))));
	}

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
