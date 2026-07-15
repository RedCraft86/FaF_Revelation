// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "StaminaWidget.h"
#include "GamePlayerCharacter.h"

UStaminaWidget::UStaminaWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	Visibility = ESlateVisibility::HitTestInvisible;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS

	BarColor = FLinearColor::White;
}

void UStaminaWidget::OnStaminaUpdate(const bool bEnabled, const FPlayerStaminaInfo& Info)
{
	SetVisibility(bEnabled ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (bEnabled)
	{
		StaminaBar->SetPercent(Info.GetStamina());
		BarColor = Info.IsPunished() ? FLinearColor::Red : FLinearColor::White;
		BarColor.A = FMath::GetMappedRangeValueClamped(
			FVector2D(0.8f, 1.0f), 
			FVector2D(1.0f, 0.1f), 
			StaminaBar->GetPercent()
		);
	}
}

void UStaminaWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	StaminaBar->SetFillColorAndOpacity(FMath::CInterpTo(
		StaminaBar->GetFillColorAndOpacity(), BarColor, InDeltaTime, 10.0f
	));
}

void UStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (AGamePlayerCharacter* Player = AGamePlayerCharacter::Get<AGamePlayerCharacter>(this))
	{
		Player->OnStaminaUpdate.AddUObject(this, &UStaminaWidget::OnStaminaUpdate);
	}
}

void UStaminaWidget::NativeDestruct()
{
	if (AGamePlayerCharacter* Player = AGamePlayerCharacter::Get<AGamePlayerCharacter>(this))
	{
		Player->OnStaminaUpdate.RemoveAll(this);
	}
	Super::NativeDestruct();
}
