// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/ExpressiveTextData.h"
#include "Widgets/ExpressiveTextRendererWidget.h"
#include "ExprTextBlock.generated.h"

UCLASS(DisplayName = "Expressive Text")
class TORORUNTIME_API UExprTextBlock final : public UExpressiveTextRendererWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Content", meta = (ShowOnlyInnerProperties))
		FExpressiveTextData ExpressiveText;

	UFUNCTION(BlueprintCallable, Category = "Expressive Text Block")
		void SetText(const FText InText, const bool bUseFieldsFromAsset = true);

	UFUNCTION(BlueprintCallable, Category = "Expressive Text Block")
		void SetTextFields(const FExpressiveTextFields& InFields);

	UFUNCTION(BlueprintCallable, Category = "Expressive Text Block")
		void SetTextAsset(UExpressiveTextAsset* InAsset);

	// Run with tick to update any size changes to this text
	UFUNCTION(BlueprintCallable, Category = "Expressive Text Block")
		void UpdateSize();
	
private:

	UPROPERTY() int64 CachedChecksum = 0;
	UPROPERTY() FVector2D CurrentSize{0.0f};
	UPROPERTY() FVector2D CachedSize{0.0f};

	void UpdateText();
	bool IsCacheInvalid() const;
	virtual void SynchronizeProperties() override;
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override
	{
		return NSLOCTEXT("UMG", "Common", "Common");
	}
	virtual void OnCreationFromPalette() override
	{
		SetText(NSLOCTEXT("Toro", "ExprTextBlock", "[32rr](Expressive Text)"));
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
#endif
};
