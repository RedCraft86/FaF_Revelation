// Copyright (C) RedCraft86. All Rights Reserved.

#include "ExprTextTypes.h"
#include "Styles/ExpressiveTextStyle.h"
#include "ToroCore.h"

#define GetExprDefault LoadObject<UExpressiveTextStyle>(nullptr, \
	TEXT("/Game/AssetPacks/ExpressiveText/TextStyles/ETS_Default.ETS_Default"));

FExpressiveTextData::FExpressiveTextData() : bUseAsset(false), TextFields({}), TextAsset(nullptr)
{
	TextFields.DefaultFontSize = 24;
	TextFields.UseDefaultFontSize = true;
	TextFields.WrapSettings.Value = 1.0f;
	TextFields.WrapSettings.ValueType = EExpressiveTextWrapMode::WrapAtPercentageOfParentSize;
	TextFields.DefaultStyle = GetExprDefault;
}

int64 FExpressiveTextData::CalcChecksum() const
{
	int64 Result = TextFields.CalcChecksum();
	Result = HashCombine(Result, GetTypeHash(bUseAsset));
	Result = HashCombine(Result, GetTypeHash(TextAsset));
	if (TextAsset) Result = HashCombine(Result, TextAsset->CalcChecksum());
	return Result;
}

FExpressiveText FExpressiveTextData::GetExpressiveText()
{
	FExpressiveText Result;
	if (bUseAsset)
	{
		if (TextAsset) Result.SetFields(TextAsset->Fields);
#if WITH_EDITOR
		else UE_LOG(LogToroCore, Warning, TEXT("[ExprTextTypes.cpp:31] ")
			TEXT("Cannot use TextAsset, it is NULL"))
#endif
	}
	else
	{
		Result.SetFields(TextFields);
		if (!Result.GetDefaultStyle())
		{
			TextFields.DefaultStyle = GetExprDefault;
			Result.SetDefaultStyle(TextFields.DefaultStyle);
		}
	}
	
	return Result;
}

void FExpressiveTextData::SetText(const FText& InText, const bool bUseFieldsFromAsset)
{
	if (bUseFieldsFromAsset && TextAsset)
	{
		SetTextFields(TextAsset->Fields);
	}

	bUseAsset = false;
	TextFields.Text = InText;
}

void FExpressiveTextData::SetTextFields(const FExpressiveTextFields& InFields)
{
	bUseAsset = false;
	TextFields = InFields;
}

void FExpressiveTextData::SetTextAsset(UExpressiveTextAsset* InAsset)
{
	bUseAsset = true;
	TextAsset = InAsset;
}

FExpressiveText UExprTextDataLibrary::GetExpressiveText(FExpressiveTextData& InData)
{
	return InData.GetExpressiveText();
}

void UExprTextDataLibrary::SetText(FExpressiveTextData& InData, const FText InText, const bool bUseFieldsFromAsset)
{
	InData.SetText(InText, bUseFieldsFromAsset);
}

void UExprTextDataLibrary::SetTextFields(FExpressiveTextData& InData, const FExpressiveTextFields& InFields)
{
	InData.SetTextFields(InFields);
}

void UExprTextDataLibrary::SetTextAsset(FExpressiveTextData& InData, UExpressiveTextAsset* InAsset)
{
	InData.SetTextAsset(InAsset);
}