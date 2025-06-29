// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/TextBlock.h"
#include "SaveSystem/ToroSaveTypes.h"
#include "UserSettings/ToroUserSettings.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "AppStatsWidget.generated.h"

UCLASS(Abstract)
class TOROUTILITIES_API UAppStatsWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UAppStatsWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> UnfocusedView;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> FrameRateText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DeltaTimeText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SaveAnim;

private:

	UPROPERTY() float TargetFPS;
	UPROPERTY() float UpdateTick;

	void UpdateFrameRate() const;
	void OnGameFocusChanged(const bool bFocused) const;
	void OnSettingsChanged(const UToroUserSettings* Settings);
	void OnSaveLoad(const UToroSaveObject* SaveObject, const ESaveGameActivity Activity);

	virtual void InitWidget() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
