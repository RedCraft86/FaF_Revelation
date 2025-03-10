// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenu.h"
#include "EnhancedCodeFlow.h"
#include "Components/TextBlock.h"
#include "GeneralProjectSettings.h"
#include "Characters/ToroPlayerBase.h"
#include "Framework/ToroPlayerController.h"
#include "UserSettings/SettingsWidgetBase.h"

UMainMenuBase::UMainMenuBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bGameplayOnly = false;
}

void UMainMenuBase::OnPlayClicked()
{
	DeactivateWidget();
	PlayerChar->FadeToBlack(1.0f, true);
	FFlow::Delay(this, 1.0f, [this]
	{	
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, GameplayLevel);
	});
}

void UMainMenuBase::OnSettingsClicked()
{
	SettingsWidget->ActivateWidget();
	PlayerChar->GetPlayerController()->SetGameInputMode(EGameInputMode::GameAndUI, true,
		EMouseLockMode::LockAlways, false, SettingsWidget);
	SetHidden(true);
}

void UMainMenuBase::OnExtrasClicked()
{
	DeactivateWidget();
	PlayerChar->FadeToBlack(1.0f, true);
	FFlow::Delay(this, 1.0f, [this]
	{	
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, ExtrasLevel);
	});
}

void UMainMenuBase::OnQuitClicked()
{
	DeactivateWidget();
	PlayerChar->FadeToBlack(1.0f, true);
	FFlow::Delay(this, 1.0f, [this]
	{	
		UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
	});
}

void UMainMenuBase::InitWidget()
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

void UMainMenuBase::Return_Implementation()
{
	SetHidden(false);
	PlayerChar->GetPlayerController()->SetGameInputMode(EGameInputMode::GameAndUI, true,
		EMouseLockMode::LockAlways, false, this);
}
