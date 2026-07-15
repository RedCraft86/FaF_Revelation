// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "DataTypes/MessagingTypes.h"
#include "DialogueWidget.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class UDialogWidget final : public UUserWidget
{
	GENERATED_BODY()

public:

	UDialogWidget(const FObjectInitializer& ObjectInit);

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<class UTextBlock> SpeakerLabel;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<class UExpressiveTextBlock> SubtitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FadeAnim;

	void OnSubtitleMessage(const FSubtitleMessage& InSubtitle);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
