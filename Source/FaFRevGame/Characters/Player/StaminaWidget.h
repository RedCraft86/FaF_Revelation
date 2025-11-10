// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PlayerCharacter.h"
#include "Components/ProgressBar.h"
#include "UserInterface/ToroManagedWidget.h"
#include "StaminaWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UStaminaWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UStaminaWidget(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UProgressBar> NormalBar;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UProgressBar> ReserveBar;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		FLinearColor ReserveFullColor;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		FLinearColor ReserveNonFullColor;

	TObjectPtr<APlayerCharacter> Player;

	void OnStaminaUpdate(const bool bEnabled, const FPlayerStamina& StaminaInfo);

	virtual bool ShouldHideWidget() const override;
	virtual void InitWidget(APlayerController* Controller) override;
};
