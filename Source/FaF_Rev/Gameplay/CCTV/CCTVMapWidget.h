// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CCTVMapWidget.generated.h"

UCLASS(Abstract)
class FAF_REV_API UCCTVMapWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class ACCTVMonitor;

public:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "CCTVMapWidget", meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FadeAnim;

	UFUNCTION(BlueprintCallable, Category = "CCTVMapWidget")
		void SetSelectedCamera(const FName InID);

protected:

	UPROPERTY(Transient)
		TObjectPtr<ACCTVMonitor> OwningMonitor;

	void RemoveWidget();
	virtual void NativeConstruct() override;
};
