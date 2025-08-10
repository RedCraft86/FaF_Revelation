// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "SubtitleWidget.generated.h"

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

	bool IsValidData() const { return !Line.IsEmptyOrWhitespace() && ExtraTime >= 0.0f; }
	float CalcTime() const { return ExtraTime + UToroUtilSettings::Get()->GetReadingTime(Line.ToString()); }
};

UCLASS(Abstract)
class TOROUTILITIES_API USubtitleWidget : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	USubtitleWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UTextBlock> NameText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UExprTextBlock> LineText;

	UPROPERTY(EditAnywhere, Category = Settings)
		FLinearColor PlayerColor;

	UFUNCTION(BlueprintCallable, Category = Subtitles, meta = (WorldContext = "ContextObject"))
		static void QueueSubtitle(const UObject* ContextObject, const FToroSubtitle& InSubtitle);

	UFUNCTION(BlueprintCallable, Category = Subtitles, meta = (WorldContext = "ContextObject"))
		static void QueueSubtitles(const UObject* ContextObject, const TArray<FToroSubtitle> InSubtitles);

private:

	UPROPERTY() FTimerHandle SubtitleTimer;
	UPROPERTY() TArray<FToroSubtitle> Subtitles;

	void NextSubtitle();
	void ShowSubtitle(const FToroSubtitle& InData);
	void AddSubtitle(const FToroSubtitle& InSubtitle);
	void AddSubtitles(const TArray<FToroSubtitle>& InSubtitles);
	void OnNativeSubtitle(const FText& InText);

	virtual void InitWidget() override;
	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsPlayableMap(this); }
};
