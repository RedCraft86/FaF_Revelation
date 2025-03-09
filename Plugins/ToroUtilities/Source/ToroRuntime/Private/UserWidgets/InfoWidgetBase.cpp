// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/InfoWidgetBase.h"
#include "UserSettings/ToroUserSettings.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

FLinearColor CalcFrameRateColor(const FLinearColor& Good, const FLinearColor& Bad, const float Target, const float Current)
{
	return FMath::Lerp(Bad, Good, FMath::GetMappedRangeValueClamped(
		FVector2D(0.0f, (FMath::IsNearlyZero(Target) ? 60.0f : Target) - 10.0f),
		FVector2D(0.0f, 1.0f), Current));
}

UInfoWidgetBase::UInfoWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	, GoodFrameRateColor(FLinearColor::Green), BadFrameRateColor(FLinearColor::Red)
	, bShowFPS(false), FPSTick(0.0f), TargetFPS(60.0f)
{
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInfoWidgetBase::UpdateFrameRate() const
{
	const int32 FPS = FMath::RoundToInt32(1.0 / FApp::GetDeltaTime());
	const FLinearColor Color = CalcFrameRateColor(GoodFrameRateColor, BadFrameRateColor, TargetFPS, FPS);
	
	FrameRateText->SetText(FText::FromString(FString::Printf(TEXT("%d FPS"), FPS)));
	FrameRateText->SetColorAndOpacity(Color);

	DeltaTimeText->SetText(FText::FromString(FString::Printf(TEXT("%.2f ms"), FApp::GetDeltaTime() * 1000.0f)));
	DeltaTimeText->SetColorAndOpacity(Color);
}

void UInfoWidgetBase::MarkUnfocused(const bool bUnfocused) const
{
	UnfocusedView->SetVisibility(bUnfocused ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UInfoWidgetBase::OnSettingsChanged(const UToroUserSettings* Settings)
{
	TargetFPS = Settings->GetFrameRateLimit();
	if (TargetFPS > 150.0f) TargetFPS = 60.0f;
	bShowFPS = Settings->GetShowFPS();
	UpdateFrameRate();

	FrameRateText->SetVisibility(bShowFPS ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	DeltaTimeText->SetVisibility(bShowFPS ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UInfoWidgetBase::OnSaveLoad(const UToroUserSettings* Settings)
{
	PlayAnimation(SaveAnim);
	OnSettingsChanged(Settings ? Settings : UToroUserSettings::Get());
}

void UInfoWidgetBase::InitWidget()
{
	Super::InitWidget();
	UnfocusedView->SetVisibility(ESlateVisibility::Collapsed);
	if (UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		Settings->OnSettingsApplied.AddUObject(this, &UInfoWidgetBase::OnSaveLoad);
		Settings->OnDynamicSettingsChanged.AddUObject(this, &UInfoWidgetBase::OnSettingsChanged);
		OnSettingsChanged(Settings);
	}
}

void UInfoWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bShowFPS) return;
	if (FPSTick >= 0.1f)
	{
		FPSTick = 0.0f;
		UpdateFrameRate();
	}
	else { FPSTick += InDeltaTime; }
}
