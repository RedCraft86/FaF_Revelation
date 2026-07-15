// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class UInteractionWidget final : public UUserWidget
{
	GENERATED_BODY()

public:

	UInteractionWidget(const FObjectInitializer& ObjectInit);

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UWidget> InvalidTarget;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UWidget> ValidTarget;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<class UTextBlock> TargetLabel;

	void OnTargetChanged(const TWeakObjectPtr<AActor>& Target) const;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
