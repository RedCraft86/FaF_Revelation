// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GameContextWidget.h"
#include "CommonButtonBase.h"
#include "Framework/ToroPlayerHUD.h"
#include "Framework/ToroGameViewportClient.h"
#include "GamePlayer/GamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/LatentInfo.h"
#include "GameSettings.h"

UGameContextWidget::UGameContextWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UGameContextWidget::OnResumeClicked() const
{
	if (AGamePlayerController* PC = AGamePlayerController::Get<AGamePlayerController>(this))
	{
		PC->SetGamePaused(false);
	}
}

void UGameContextWidget::OnSettingsClicked() const
{
	if (const AToroPlayerHUD* HUD = AToroPlayerHUD::Get<AToroPlayerHUD>(this))
	{
		HUD->PushSettings();
	}
}

void UGameContextWidget::OnCheckpointClicked()
{
	OpenMap(EGameMapType::Gameplay);
}

void UGameContextWidget::OnMainMenuClicked()
{
	OpenMap(EGameMapType::MainMenu);
}

UE5Coro::TCoroutine<> UGameContextWidget::OpenMap(const EGameMapType Map)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	co_await UToroGameViewportClient::StartScreenFade(FLatentInfo::Make(), this, FLinearColor::Black, 1.0f, true);
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, UGameSettings::Get()->GameMaps[static_cast<uint8>(Map)]);
}

void UGameContextWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ResumeButton->OnClicked().AddUObject(this, &UGameContextWidget::OnResumeClicked);
	SettingsButton->OnClicked().AddUObject(this, &UGameContextWidget::OnSettingsClicked);
	CheckpointButton->OnClicked().AddUObject(this, &UGameContextWidget::OnCheckpointClicked);
	MainMenuButton->OnClicked().AddUObject(this, &UGameContextWidget::OnMainMenuClicked);
}
