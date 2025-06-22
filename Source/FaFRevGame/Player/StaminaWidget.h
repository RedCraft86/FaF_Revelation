// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "GamePlayerChar.h"
#include "Components/ProgressBar.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "StaminaWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UStaminaWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UStaminaWidget(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), StaminaBarSpeed(0.1f, 10.0f)
	{
		ZOrder = 40;
	}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.05f, UIMin = 0.05f))
		FVector2D StaminaBarSpeed;

private:

	UPROPERTY(Transient) TObjectPtr<AGamePlayerChar> Player;

	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnGameplayMap(this); }
	virtual bool ShouldBeHidden() override
	{
		return Super::ShouldBeHidden() && Player ? Player->HasControlFlag(PCF_UseStamina) : false;
	}

	virtual void NativeConstruct() override
	{
		Super::NativeConstruct();
		Player = AGamePlayerChar::Get<AGamePlayerChar>(this);
	}

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override
	{
		Super::NativeTick(MyGeometry, DeltaTime);
		if (Player && Player->HasControlFlag(PCF_UseStamina))
		{
			StaminaBar->SetPercent(FMath::FInterpConstantTo(StaminaBar->GetPercent(),
				Player->GetStaminaPercent(), DeltaTime, FMath::Abs(
					Player->GetStaminaDelta()) * StaminaBarSpeed.X));

			StaminaBar->SetRenderOpacity(FMath::GetMappedRangeValueClamped(FVector2D(0.85f, 1.0f),
				FVector2D(1.0f, 0.05f), StaminaBar->GetPercent()));

			StaminaBar->SetFillColorAndOpacity(FMath::CInterpTo(StaminaBar->GetFillColorAndOpacity(),
				Player->IsStaminaPunished() ? FLinearColor::Red : FLinearColor::White,
				DeltaTime, StaminaBarSpeed.Y));
		}
	}
};
