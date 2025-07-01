// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "ExtrasWidget.generated.h"

class UButton;

UCLASS(Abstract)
class FAFREVGAME_API UExtrasWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UExtrasWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ExitButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Settings)
		TObjectPtr<UUserWidget> ParentUI;

private: // TODO

	UFUNCTION() void OnExitClicked() { DeactivateWidget(); }

	virtual void InitWidget() override;
	virtual void InternalProcessDeactivation() override;
	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnLaunchMap(this); }
};
