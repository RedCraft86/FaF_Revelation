// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Handles/ExpressiveText.h"
#include "Asset/ExpressiveTextAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExpressiveTextTypes.generated.h"

// ExpressiveTextFields -> DefaultFontSize -> Add Metadata: EditCondition = "UseDefaultFontSize"
// ExpressiveTextFields -> UseDefaultFontSize -> Add Metadata: InlineEditConditionToggle

USTRUCT(BlueprintType)
struct TORORUNTIME_API FExpressiveTextData
{
	GENERATED_BODY()

	FExpressiveTextData();

	int64 CalcChecksum() const;
	FExpressiveText GetExpressiveText();
	void SetText(const FText& InText, const bool bUseFieldsFromAsset = false);
	void SetTextFields(const FExpressiveTextFields& InFields);
	void SetTextAsset(UExpressiveTextAsset* InAsset);
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ExpressiveText, meta = (AllowPrivateAccess = true))
		bool bUseAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ExpressiveText, meta = (EditCondition = "!bUseAsset", EditConditionHides, AllowPrivateAccess = true, ShowOnlyInnerProperties))
		FExpressiveTextFields TextFields;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ExpressiveText, meta = (EditCondition = "bUseAsset", EditConditionHides, AllowPrivateAccess = true))
		TObjectPtr<UExpressiveTextAsset> TextAsset;
};

UCLASS(DisplayName = "Expressive Text Data")
class TORORUNTIME_API UExprTextDataLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = ExpressiveTextData)
		static FExpressiveText GetExpressiveText(UPARAM(ref) FExpressiveTextData& InData);

	UFUNCTION(BlueprintCallable, Category = ExpressiveTextData)
		static void SetText(UPARAM(ref) FExpressiveTextData& InData, const FText InText, const bool bUseFieldsFromAsset = false);

	UFUNCTION(BlueprintCallable, Category = ExpressiveTextData)
		static void SetTextFields(UPARAM(ref) FExpressiveTextData& InData, const FExpressiveTextFields& InFields);
	
	UFUNCTION(BlueprintCallable, Category = ExpressiveTextData)
		static void SetTextAsset(UPARAM(ref) FExpressiveTextData& InData, UExpressiveTextAsset* InAsset);
};