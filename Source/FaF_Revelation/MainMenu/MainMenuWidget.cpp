// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenuWidget.h"
#include "EnhancedCodeFlow.h"
#include "Components/TextBlock.h"
#include "GeneralProjectSettings.h"
#include "Characters/ToroPlayerBase.h"
#include "Framework/ToroPlayerController.h"
#include "UserSettings/SettingsWidgetBase.h"

UMainMenuWidgetBase::UMainMenuWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ZOrder = 50;
	bGameplayOnly = false;
}

void UMainMenuWidgetBase::OnPlayClicked()
{
	DeactivateWidget();
	PlayerChar->FadeToBlack(1.0f, true);
	FFlow::Delay(this, 1.0f, [this]
	{	
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, GameplayLevel);
	});
}

void UMainMenuWidgetBase::OnSettingsClicked()
{
	SettingsWidget->ActivateWidget();
	PlayerChar->GetPlayerController()->SetGameInputMode(EGameInputMode::GameAndUI, true,
		EMouseLockMode::LockAlways, false, SettingsWidget);
	SetHidden(true);
}

void UMainMenuWidgetBase::OnExtrasClicked()
{
	DeactivateWidget();
	PlayerChar->FadeToBlack(1.0f, true);
	FFlow::Delay(this, 1.0f, [this]
	{	
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, ExtrasLevel);
	});
}

void UMainMenuWidgetBase::OnQuitClicked()
{
	DeactivateWidget();
	PlayerChar->FadeToBlack(1.0f, true);
	FFlow::Delay(this, 1.0f, [this]
	{	
		UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
	});
}

void UMainMenuWidgetBase::InitWidget()
{
	Super::InitWidget();
	PlayerChar = AToroPlayerBase::Get(this);
	Return_Implementation();

	SettingsWidget = CreateNew<USettingsWidgetBase>(PlayerChar->GetPlayerController(), SettingsWidgetClass);
	SettingsWidget->ParentUI = this;

	if (const UGeneralProjectSettings* ProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		GameVersionText->SetText(FText::FromString(FString::Printf(TEXT("Game Version: %s | %s Build"),
			*ProjectSettings->ProjectVersion, LexToString(FApp::GetBuildConfiguration()))));
	}
}

void UMainMenuWidgetBase::Return_Implementation()
{
	SetHidden(false);
	PlayerChar->GetPlayerController()->SetGameInputMode(EGameInputMode::GameAndUI, true,
		EMouseLockMode::LockAlways, false, this);
}
