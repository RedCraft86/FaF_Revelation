// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntime.h"
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
	virtual bool CanCreateWidget(const UObject* ContextObject) const override
	{
		return Super::CanCreateWidget(ContextObject) &&
			!UToroSettings::Get()->IsOnMap(ContextObject, EToroMapType::MainMenu);
	}
};
