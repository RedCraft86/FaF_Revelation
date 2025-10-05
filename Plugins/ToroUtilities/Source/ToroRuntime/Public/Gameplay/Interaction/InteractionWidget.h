// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InteractionData.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UserInterface/ToroManagedWidget.h"
#include "InteractionWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UInteractionWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UInteractionWidget(const FObjectInitializer& ObjectInitializer);

	void SetInteractionInfo(const FInteractionInfo& Info) const;

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidgetSwitcher> IconSwitch;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> InteractText;
};
