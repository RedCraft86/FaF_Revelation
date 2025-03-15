// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/PauseWidgetBase.h"
#include "UserSettings/SettingsWidgetBase.h"
#include "Framework/ToroPlayerController.h"
#include "SaveSystem/ToroSaveManager.h"
#include "SaveSystem/BaseSaveObjects.h"
#include "Characters/ToroPlayerBase.h"
#include "GeneralProjectSettings.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "EnhancedCodeFlow.h"

UPauseWidgetBase::UPauseWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), SaveTag(Tag_GameSave)
{
	ZOrder = 58;
	bAutoActivate = false;
}

void UPauseWidgetBase::OnResumeClicked()
{
	PlayerChar->GetPlayerController()->SetPauseState(false);
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPauseWidgetBase::OnSettingsClicked()
{
	SettingsWidget->ActivateWidget();
	PlayerChar->GetPlayerController()->SetGameInputMode(EGameInputMode::GameAndUI, true,
		EMouseLockMode::LockAlways, false, SettingsWidget);
	SetHidden(true);
}

void UPauseWidgetBase::OnCheckpointClicked()
{
	if (UGameSaveObjectBase* SaveObject = ToroSave::GetObject<UGameSaveObjectBase>(this, SaveTag))
	{
		SaveObject->SaveObject(nullptr);
	}

	SetHidden(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	FFlow::Delay(this, 2.0f, [this]()
	{
		UGameplayStatics::OpenLevel(this, *UGameplayStatics::GetCurrentLevelName(this));
	});
}

void UPauseWidgetBase::OnMainMenuClicked()
{
	if (UGameSaveObjectBase* SaveObject = ToroSave::GetObject<UGameSaveObjectBase>(this, SaveTag))
	{
		SaveObject->SaveObject(nullptr);
	}

	SetHidden(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	FFlow::Delay(this, 2.0f, [this]()
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainMenuLevel);
	});
}

void UPauseWidgetBase::InitWidget()
{
	Super::InitWidget();
	PlayerChar = AToroPlayerBase::Get(this);
	Return_Implementation();

	ResumeButton->OnClicked.AddUniqueDynamic(this, &UPauseWidgetBase::OnResumeClicked);
	SettingsButton->OnClicked.AddUniqueDynamic(this, &UPauseWidgetBase::OnSettingsClicked);
	CheckpointButton->OnClicked.AddUniqueDynamic(this, &UPauseWidgetBase::OnCheckpointClicked);
	MainMenuButton->OnClicked.AddUniqueDynamic(this, &UPauseWidgetBase::OnMainMenuClicked);

	SettingsWidget = CreateNew<USettingsWidgetBase>(PlayerChar->GetPlayerController(), SettingsWidgetClass);
	SettingsWidget->ParentUI = this;

	if (const UGeneralProjectSettings* ProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		GameVersionText->SetText(FText::FromString(FString::Printf(TEXT("Game Version: %s | %s Build"),
			*ProjectSettings->ProjectVersion, LexToString(FApp::GetBuildConfiguration()))));
	}
}

void UPauseWidgetBase::Return_Implementation()
{
	SetHidden(false);
	PlayerChar->GetPlayerController()->SetGameInputMode(EGameInputMode::GameAndUI, true,
		EMouseLockMode::LockAlways, false, this);
}

void UPauseWidgetBase::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	PlayerChar->GetPlayerController()->SetGameInputMode(EGameInputMode::GameOnly);
}
