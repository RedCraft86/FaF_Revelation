// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenuWidget.h"
#include "EnhancedCodeFlow.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "UserInterface/NativeContainers.h"
#include "UserInterface/ToroWidgetManager.h"
#include "Framework/ToroPlayerController.h"
#include "GamePhase/GamePhaseManager.h"
#include "Helpers/WidgetAnimHelpers.h"
#include "MainMenuActor.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
	ContainerClass = UMenuWidgetContainer::StaticClass();
	Themes.Add(MenuThemeTags::TAG_Default.GetTag(), TEXT("Default"));
}

void UMainMenuWidget::ChangeMenuTheme(const FGameplayTag ThemeTag)
{
	PopWidget();
	SetVisibility(ESlateVisibility::HitTestInvisible);
	if (MenuActor) MenuActor->SetMenuTheme(ThemeTag);
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
		UKismetSystemLibrary::QuitGame(this, AToroPlayerController::Get(this),
			EQuitPreference::Quit, false);
	});
}

void UMainMenuWidget::OnThemePicked(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!MenuActor) return;
	const FGameplayTag* TargetTheme = Themes.FindKey(SelectedItem);
	if (TargetTheme && MenuActor->GetMenuTheme() != *TargetTheme)
	{
		ChangeMenuTheme(*TargetTheme);
	}
	else
	{
		ThemeDropdown->SetSelectedOption(Themes.FindRef(MenuActor->GetMenuTheme()));
	}
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
	VersionText->SetText(UToroSettings::Get()->GetVersionLabel());
	MenuThemeBox->SetVisibility(ESlateVisibility::Collapsed);
	if (MenuActor)
	{
		const TArray<FGameplayTag>& AvailableThemes = MenuActor->GetThemes();
		if (!AvailableThemes.IsEmpty())
		{
			ThemeDropdown->ClearOptions();
			MenuThemeBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			for (auto It = Themes.CreateIterator(); It; ++It)
			{
				if (AvailableThemes.Contains(It.Key()))
				{
					ThemeDropdown->AddOption(It.Value());
				}
				else
				{
					It.RemoveCurrent();
				}
			}
			ThemeDropdown->SetSelectedOption(Themes.FindRef(MenuActor->GetMenuTheme()));
		}
	}
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->SetInputConfig({EGameInputMode::UI_Only, true,
			EMouseLockMode::LockAlways, false});
	}
}

void UMainMenuWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	PlayButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnPlayButton);
	SettingsButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnSettingsButton);
	ExtrasButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnExtrasButton);
	QuitButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnQuitButton);
	ThemeDropdown->OnSelectionChanged.AddUniqueDynamic(this, &UMainMenuWidget::OnThemePicked);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
		{
			Settings = Manager->FindWidget<USettingsWidget>();
			Difficulty = Manager->FindWidget<UDifficultyWidget>();
		}
	});
}
