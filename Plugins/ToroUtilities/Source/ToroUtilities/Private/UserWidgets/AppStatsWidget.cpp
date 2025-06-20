// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/AppStatsWidget.h"
#include "Framework/ToroPlayerController.h"
#include "SaveSystem/ToroSaveManager.h"
#include "Components/PanelWidget.h"

FLinearColor CalcFrameRateColor(const float Target, const float Current)
{
	return FMath::Lerp(FLinearColor::Red, FLinearColor::Green, FMath::GetMappedRangeValueClamped(
			FVector2D(0.0f, FMath::IsNearlyZero(Target) ? 50.0f : Target - 10.0f),
		FVector2D(0.0f, 1.0f), Current));
}

UAppStatsWidget::UAppStatsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), TargetFPS(60.0f), UpdateTick(0.0f)
{
	ZOrder = 100;
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UAppStatsWidget::UpdateFrameRate() const
{
	const int32 FPS = FMath::RoundToInt32(1.0 / FApp::GetDeltaTime());
	const FLinearColor Color = CalcFrameRateColor(TargetFPS, FPS);
	
	FrameRateText->SetText(FText::FromString(FString::Printf(TEXT("%d FPS"), FPS)));
	FrameRateText->SetColorAndOpacity(Color);

	DeltaTimeText->SetText(FText::FromString(FString::Printf(TEXT("%.2f ms"), FApp::GetDeltaTime() * 1000.0f)));
	DeltaTimeText->SetColorAndOpacity(Color);
}

void UAppStatsWidget::OnGameFocusChanged(const bool bFocused) const
{
	UnfocusedView->SetVisibility(bFocused ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void UAppStatsWidget::OnSettingsChanged(const UToroUserSettings* Settings)
{
	TargetFPS = Settings->GetFrameRateLimit();
	if (TargetFPS > 150.0f) TargetFPS = 60.0f;

	const bool bShowFPS = Settings->GetShowFPS();
	FrameRateText->SetVisibility(bShowFPS ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	DeltaTimeText->SetVisibility(bShowFPS ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	UpdateFrameRate();
}

void UAppStatsWidget::OnSaveLoad(const UToroSaveObject* SaveObject, const ESaveGameActivity Activity)
{
	if (SaveObject && Activity == ESaveGameActivity::Saving)
	{
		PlayAnimation(SaveAnim, 0.0f, 5);
	}
}

void UAppStatsWidget::InitWidget()
{
	Super::InitWidget();
	UnfocusedView->SetVisibility(ESlateVisibility::Collapsed);
	if (UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		Settings->OnSettingsApplied.AddUObject(this, &UAppStatsWidget::OnSettingsChanged);
		Settings->OnDynamicSettingsChanged.AddUObject(this, &UAppStatsWidget::OnSettingsChanged);
		OnSettingsChanged(Settings);
	}

	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->OnGameFocusChanged.AddUObject(this, &UAppStatsWidget::OnGameFocusChanged);
	}

	if (UToroSaveManager* SaveManager = UToroSaveManager::Get(this))
	{
		SaveManager->OnSaveIO.AddUObject(this, &UAppStatsWidget::OnSaveLoad);
	}
}

void UAppStatsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UpdateTick >= 0.1f)
	{
		UpdateTick = 0.0f;
		UpdateFrameRate();
	}
	else { UpdateTick += InDeltaTime; }
}
