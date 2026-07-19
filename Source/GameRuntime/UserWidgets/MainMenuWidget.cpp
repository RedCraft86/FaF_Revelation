// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "MainMenuWidget.h"
#include "CommonButtonBase.h"
#include "Framework/ToroPlayerHUD.h"
#include "Framework/ToroGameViewportClient.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/LatentInfo.h"
#include "GameSettings.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UMainMenuWidget::OnPlayClicked()
{
	FadeOut([this]()
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, 
			UGameSettings::Get()->GameMaps[static_cast<uint8>(EGameMapType::Gameplay)]
		);
	});
}

void UMainMenuWidget::OnSettingsClicked() const
{
	if (const AToroPlayerHUD* HUD = AToroPlayerHUD::Get<AToroPlayerHUD>(this))
	{
		HUD->PushSettings();
	}
}

void UMainMenuWidget::OnExtrasClicked()
{
	FadeOut([this]()
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, 
			UGameSettings::Get()->GameMaps[static_cast<uint8>(EGameMapType::MainMenu)]
		);
	});
}

void UMainMenuWidget::OnQuitClicked()
{
	FadeOut([this]()
	{
		UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
	});
}

UE5Coro::TCoroutine<> UMainMenuWidget::FadeOut(const TFunction<void()>& Callback)
{
	if (Callback)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
		co_await UToroGameViewportClient::StartScreenFade(FLatentInfo::Make(TEXT(""), this), this, FLinearColor::Black, 1.0f, true);
		Callback();
	}
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayButton->OnClicked().AddUObject(this, &UMainMenuWidget::OnPlayClicked);
	SettingsButton->OnClicked().AddUObject(this, &UMainMenuWidget::OnSettingsClicked);
	ExtrasButton->OnClicked().AddUObject(this, &UMainMenuWidget::OnExtrasClicked);
	QuitButton->OnClicked().AddUObject(this, &UMainMenuWidget::OnQuitClicked);
}
