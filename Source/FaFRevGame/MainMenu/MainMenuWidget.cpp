// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenuWidget.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "UserInterface/NativeContainers.h"
#include "UserInterface/ToroWidgetManager.h"
#include "Framework/ToroPlayerController.h"
#include "MusicSystem/WorldMusicManager.h"
#include "GamePhase/GamePhaseManager.h"
#include "Helpers/WidgetAnimHelpers.h"
#include "MainMenuActor.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
	ContainerClass = UMenuWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::Visible);
	ThemeNames.Add(MenuThemeTags::TAG_Default.GetTag(), TEXT("Default"));
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

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
	{
		if (const TSoftObjectPtr<UWorld>* Map = UToroSettings::Get()->MapRegistry.Find(EToroMapType::Extras))
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, *Map);
		}
	}, 0.6f, false);
}

void UMainMenuWidget::OnQuitButton()
{
	SetHidden(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	UToroShortcutLibrary::StartCameraFade(this, 0.0f, 1.0f, 0.5f);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
	{
		UKismetSystemLibrary::QuitGame(this, AToroPlayerController::Get(this),
			EQuitPreference::Quit, false);
	}, 0.6f, false);
}

void UMainMenuWidget::OnThemePicked(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (MenuActor && SelectionType != ESelectInfo::Direct)
	{
		const FGameplayTag* TargetTheme = ThemeNames.FindKey(SelectedItem);
		if (TargetTheme && MenuActor->GetMenuTheme() != *TargetTheme)
		{
			ChangeMenuTheme(*TargetTheme);
		}
		else
		{
			ThemeDropdown->SetSelectedIndex(ThemeOrder.Find(MenuActor->GetMenuTheme()));
		}
	}
}

void UMainMenuWidget::OpenGameplayLevel() const
{
	UToroUserSettings::Get()->SaveSettings();
	UToroShortcutLibrary::StartCameraFade(this, 0.0f, 1.0f, 0.5f);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
	{
		if (const TSoftObjectPtr<UWorld>* Map = UToroSettings::Get()->MapRegistry.Find(EToroMapType::Gameplay))
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, *Map);
		}
	}, 0.6f, false);
}

void UMainMenuWidget::PushWidget()
{
	Super::PushWidget();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	VersionText->SetText(UToroSettings::Get()->GetVersionLabel());
	if (MenuActor && !bInit)
	{
		MenuThemeBox->SetVisibility(ESlateVisibility::Collapsed);
		const TArray<FGameplayTag>& AvailableThemes = MenuActor->GetThemes();
		if (AvailableThemes.Num() <= 1) return;

		ThemeDropdown->ClearOptions();
		MenuThemeBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		for (auto It = ThemeOrder.CreateIterator(); It; ++It)
		{
			if (AvailableThemes.Contains(*It))
			{
				ThemeDropdown->AddOption(ThemeNames.FindRef(*It));
			}
			else
			{
				It.RemoveCurrent();
			}
		}

		bInit = true;
	}

	if (const int32 ThemeIdx = ThemeOrder.Find(MenuActor->GetMenuTheme()); ThemeIdx >= 0)
	{
		ThemeDropdown->SetSelectedIndex(ThemeIdx);
		if (MusicManager)
		{
			MusicManager->SetThemeState(ThemeIdx);
		}
	}

	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->SetInputConfig({EGameInputMode::GameAndUI, true,
			EMouseLockMode::LockAlways, false});
	}

	ProgressText->SetVisibility(ESlateVisibility::Collapsed);
	if (const AToroGameState* State = AToroGameState::Get(this))
	{
		const float Progress = State->GetTotalPlayProgress() * 100.0f;
		const float PlayTime = State->GetTotalPlayTime() / 3600.0f;
		if (Progress > 0.01f && PlayTime > 0.01f)
		{
			ProgressText->SetVisibility(ESlateVisibility::HitTestInvisible);

			FNumberFormattingOptions NumberFormatOptions;
			NumberFormatOptions.MinimumIntegralDigits = 1;
			NumberFormatOptions.MaximumFractionalDigits = 2;
			NumberFormatOptions.MinimumFractionalDigits = 2;
	 
			ProgressText->SetText(FText::Format(
				INVTEXT("Progress: {0}%\nPlay Time: {1} hrs"),
				FText::AsNumber(Progress, &NumberFormatOptions),
				FText::AsNumber(PlayTime, &NumberFormatOptions)
			));
		}
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
		MusicManager = UWorldMusicManager::Get(this);
		if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
		{
			Settings = Manager->FindWidget<USettingsWidget>();
			Difficulty = Manager->FindWidget<UDifficultyWidget>();
		}
	});
}
