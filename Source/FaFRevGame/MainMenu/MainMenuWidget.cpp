// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenuWidget.h"
#include "EnhancedCodeFlow.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "UserInterface/NativeContainers.h"
#include "UserInterface/ToroWidgetManager.h"
#include "Framework/ToroPlayerController.h"
#include "Helpers/WidgetAnimHelpers.h"
#include "MainMenuActor.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
	ContainerClass = UMenuWidgetContainer::StaticClass();
}

void UMainMenuWidget::ChangeMenuTheme(const FGameplayTag EndingTag)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	if (MenuActor && MenuActor->SetMenuTheme(EndingTag))
	{
		PopWidget();
	}
	else
	{
		PlayAnimationReverse(ThemesAnim);
	}
}

void UMainMenuWidget::ShowWidget(AMainMenuActor* Actor)
{
	MenuActor = Actor;
	PushWidget();
}

void UMainMenuWidget::ReturnToWidget_Implementation(UUserWidget* FromWidget)
{
	if (FromWidget == Settings)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if (FromWidget == Difficulty)
	{
		OpenGameplayLevel();
	}
}

void UMainMenuWidget::OnPlayButton()
{
	SetHidden(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	UToroUserSettings* UserSettings = UToroUserSettings::Get();
	if (UserSettings && UserSettings->GetDifficulty() == EGameDifficulty::Unset)
	{
		UserSettings->SetDifficulty(EGameDifficulty::Normal);
		Difficulty->ShowWidget(this);
	}
	else
	{
		OpenGameplayLevel();
	}
}

void UMainMenuWidget::OnSettingsButton()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	Settings->ShowWidget(this);
}

void UMainMenuWidget::OnExtrasButton()
{
	SetHidden(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	UToroShortcutLibrary::StartCameraFade(this, 0.0f, 1.0f, 0.5f);
	FFlow::Delay(this, 0.6f, [this]()
	{
		if (const TSoftObjectPtr<UWorld>* Map = UToroSettings::Get()->MapRegistry.Find(EToroMapType::Extras))
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, *Map);
		}
	});
}

void UMainMenuWidget::OnQuitButton()
{
	SetHidden(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	UToroShortcutLibrary::StartCameraFade(this, 0.0f, 1.0f, 0.5f);
	FFlow::Delay(this, 0.6f, [this]()
	{
		UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(),
			EQuitPreference::Quit, false);
	});
}

void UMainMenuWidget::OnThemeButton()
{
	PlayAnimation(ThemesAnim);
}

void UMainMenuWidget::OpenGameplayLevel()
{
	UToroUserSettings::Get()->SaveSettings();
	UToroShortcutLibrary::StartCameraFade(this, 0.0f, 1.0f, 0.5f);
	FFlow::Delay(this, 0.6f, [this]()
	{
		if (const TSoftObjectPtr<UWorld>* Map = UToroSettings::Get()->MapRegistry.Find(EToroMapType::Gameplay))
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, *Map);
		}
	});
}

void UMainMenuWidget::PushWidget()
{
	Super::PushWidget();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	WidgetAnimHelpers::PlayOrSnapAnim(this, ThemesAnim, false, true);
	VersionText->SetText(UToroSettings::Get()->GetVersionLabel());
	ThemeButton->SetVisibility(ESlateVisibility::Collapsed);
	if (MenuActor)
	{
		const TArray<FGameplayTag>& AvailableEndings = MenuActor->GetEndings();
		if (!AvailableEndings.IsEmpty())
		{
			OnThemesAvailable(AvailableEndings);
			ThemeButton->SetVisibility(ESlateVisibility::Visible);
		}
	}
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		PC->SetInputConfig({EGameInputMode::UI_Only, true,
			EMouseLockMode::LockAlways, false, this});
	}
}

void UMainMenuWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	PlayButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnPlayButton);
	SettingsButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnSettingsButton);
	ExtrasButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnExtrasButton);
	QuitButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnQuitButton);
	ThemeButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnThemeButton);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
		{
			Settings = Manager->FindWidget<USettingsWidget>();
			Difficulty = Manager->FindWidget<UDifficultyWidget>();
		}
	});
}
