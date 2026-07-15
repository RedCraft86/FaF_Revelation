// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "DataTypes/MessagingTypes.h"
#include "NoticeWidget.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class UNoticeWidget final : public UUserWidget
{
	GENERATED_BODY()

public:

	UNoticeWidget(const FObjectInitializer& ObjectInit);

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<class UExpressiveTextBlock> NoticeText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FadeAnim;

	void OnNoticeMessage(const FGenericMessage& InNotice);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
