// Copyright (C) RedCraft86. All Rights Reserved.

#include "StaminaWidget.h"
#include "UserInterface/NativeContainers.h"

UStaminaWidget::UStaminaWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), ReserveFullColor(FLinearColor::White), ReserveNonFullColor(FLinearColor::Red)
{
	ContainerClass = UGameplayWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UStaminaWidget::OnStaminaUpdate(const bool bEnabled, const FPlayerStamina& StaminaInfo)
{
	NormalBar->SetPercent(FMath::FInterpConstantTo(
		NormalBar->GetPercent(), StaminaInfo.GetNormalPercent(),
		StaminaInfo.TickInterval, FMath::Abs(StaminaInfo.Delta))
	);
	
	ReserveBar->SetPercent(FMath::FInterpConstantTo(
		ReserveBar->GetPercent(), StaminaInfo.GetReservePercent(),
		StaminaInfo.TickInterval, FMath::Abs(StaminaInfo.Delta))
	);

	ReserveBar->SetFillColorAndOpacity(ReserveBar->GetPercent() < 0.98f? ReserveNonFullColor : ReserveFullColor);

	SetRenderOpacity(FMath::GetMappedRangeValueClamped(
		FVector2D(0.8f, 1.0f),
		FVector2D(1.0f, 0.1f),
		StaminaInfo.GetOverallPercent()
	));
}

bool UStaminaWidget::ShouldHideWidget() const
{
	return !Player || !Player->IsStaminaEnabled() || Player->IsControlLocked();
}

void UStaminaWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	NormalBar->SetPercent(1.0f);
	ReserveBar->SetPercent(1.0f);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		Player = APlayerCharacter::Get<APlayerCharacter>(this);
		Player->OnStaminaUpdate.AddUObject(this, &UStaminaWidget::OnStaminaUpdate);
	});
}
