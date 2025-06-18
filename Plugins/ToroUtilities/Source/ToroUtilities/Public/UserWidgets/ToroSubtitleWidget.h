// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "ToroSubtitleWidget.generated.h"

class UTextBlock;

USTRUCT(BlueprintType)
struct FToroSubtitle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Subtitle)
		FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Subtitle)
		FText Line;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Subtitle, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float ExtraTime;

	FToroSubtitle(): ExtraTime(0.5f) {}
	FToroSubtitle(const FText& InName, const FText& InLine, const float InExtraTime)
		: Name(InName), Line(InLine), ExtraTime(InExtraTime)
	{}

	bool IsValidSubtitle() const { return Line.IsEmptyOrWhitespace() || ExtraTime < 0.0f; }
	float CalcTime() const { return ExtraTime + UToroSettings::Get()->GetReadingTime(Line.ToString()); }
};

UCLASS(Abstract)
class TOROUTILITIES_API UToroSubtitleWidget : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UToroSubtitleWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> NameText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> LineText; // TODO UExprTextBlock

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bCaptureNativeSubtitles;

	UFUNCTION(BlueprintCallable, Category = Subtitles, meta = (WorldContext = "ContextObject"))
		static void QueueSubtitle(const UObject* ContextObject, const FToroSubtitle& InSubtitle, const bool bSmartQueue = true);

	UFUNCTION(BlueprintCallable, Category = Subtitles, meta = (WorldContext = "ContextObject"))
		static void QueueSubtitles(const UObject* ContextObject, const TArray<FToroSubtitle> InSubtitles, const bool bSmartQueue = true);

private:

	FTimerHandle SubtitleTimer;
	TArray<FToroSubtitle> Subtitles;

	void NextSubtitle();
	void ShowSubtitle(const FToroSubtitle& InData);
	void AddSubtitle(const FToroSubtitle& InSubtitle, const bool bSmartQueue);
	void AddSubtitles(const TArray<FToroSubtitle> InSubtitles, const bool bSmartQueue);

	void OnNativeSubtitle(const FText& InText);
	void SetSubtitleText(const FText& InName, const FText& InLine);

	virtual void InitWidget() override;
	virtual bool ShouldBeHidden() override { return Super::ShouldBeHidden(); } // TODO Hide on narrative dialogue
	virtual bool CanCreateWidget() const override { return UToroSettings::Get()->IsOnGameplayMap(this); }
};
