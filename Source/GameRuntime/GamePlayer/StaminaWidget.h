// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "StaminaWidget.generated.h"

UCLASS(Abstract, NotBlueprintable, BlueprintType)
class UStaminaWidget final : public UUserWidget
{
	GENERATED_BODY()

public:

	UStaminaWidget(const FObjectInitializer& ObjectInit);

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UProgressBar> StaminaBar;

	FLinearColor BarColor;

	void OnStaminaUpdate(const bool bEnabled, const struct FPlayerStaminaInfo& Info);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
