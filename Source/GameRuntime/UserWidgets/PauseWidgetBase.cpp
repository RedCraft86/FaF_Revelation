// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/PauseWidgetBase.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroMusicManager.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "SettingsWidgetBase.h"
#include "EnhancedCodeFlow.h"
#include "ToroSettings.h"

UPauseWidgetBase::UPauseWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ZOrder = 48;
	bAutoActivate = false;
}

void UPauseWidgetBase::OnResumeClicked()
{
	Player->GetPlayerController()->SetGamePaused(false);
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPauseWidgetBase::OnSettingsClicked()
{
	Settings->ActivateWidget();
	Player->GetPlayerController()->SetGameInputMode(EGameInputMode::GameAndUI, true,
		EMouseLockMode::LockAlways, false, Settings);
	SetHidden(true);
}

void UPauseWidgetBase::OnCheckpointClicked()
{
	SetHidden(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	if (AToroMusicManager* MusicManager = AToroMusicManager::Get(this))
	{
		MusicManager->SetThemeMuted(true, 2.0f);
	}
	FFlow::Delay(this, 2.0f, [this]()
	{
		UGameplayStatics::OpenLevel(this, *UGameplayStatics::GetCurrentLevelName(this));
	});
}

void UPauseWidgetBase::OnMainMenuClicked()
{
	SetHidden(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	if (AToroMusicManager* MusicManager = AToroMusicManager::Get(this))
	{
		MusicManager->SetThemeMuted(true, 2.0f);
	}
	FFlow::Delay(this, 2.0f, [this]()
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainMenuLevel);
	});
}

void UPauseWidgetBase::OnPause(const bool bPaused)
{
	bPaused ? ActivateWidget() : DeactivateWidget();
}

void UPauseWidgetBase::InitWidget()
{
	Super::InitWidget();
	Player = AToroPlayerCharacter::Get(this);
	if (AToroPlayerController* PC = Player->GetPlayerController())
	{
		PC->OnGamePaused.AddUObject(this, &UPauseWidgetBase::OnPause);
	}
	Return_Implementation();

	ResumeButton->OnClicked.AddUniqueDynamic(this, &UPauseWidgetBase::OnResumeClicked);
	SettingsButton->OnClicked.AddUniqueDynamic(this, &UPauseWidgetBase::OnSettingsClicked);
	CheckpointButton->OnClicked.AddUniqueDynamic(this, &UPauseWidgetBase::OnCheckpointClicked);
	MainMenuButton->OnClicked.AddUniqueDynamic(this, &UPauseWidgetBase::OnMainMenuClicked);

	Settings = Cast<USettingsWidgetBase>(CreateToroWidget(GetOwningPlayer(), SettingsWidgetClass));
	Settings->ParentUI = this;

	GameVersionText->SetText(UToroSettings::Get()->GetVersionLabel());
}

void UPauseWidgetBase::Return_Implementation()
{
	SetHidden(false);
	Player->GetPlayerController()->SetGameInputMode(EGameInputMode::GameAndUI, true,
		EMouseLockMode::LockAlways, false, this);
}

void UPauseWidgetBase::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	Player->GetPlayerController()->SetGameInputMode(EGameInputMode::GameOnly);
}

bool UPauseWidgetBase::CanCreateWidget() const
{
	return UToroSettings::Get()->IsOnGameplayMap(this);
}
