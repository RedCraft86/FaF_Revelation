// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/ExprTextBlock.h"

void UExprTextBlock::SetText(const FText InText, const bool bUseFieldsFromAsset)
{
	ExpressiveText.SetText(InText, bUseFieldsFromAsset);
	UpdateText();
}

void UExprTextBlock::SetTextFields(const FExpressiveTextFields& InFields)
{
	ExpressiveText.SetTextFields(InFields);
	UpdateText();
}

void UExprTextBlock::SetTextAsset(UExpressiveTextAsset* InAsset)
{
	ExpressiveText.SetTextAsset(InAsset);
	UpdateText();
}

void UExprTextBlock::UpdateSize()
{
	CurrentSize = GetCachedGeometry().GetLocalSize();
	if (!IsCacheInvalid()) return;
	UpdateText();
}

void UExprTextBlock::UpdateText()
{
	CachedSize = CurrentSize;
	CachedChecksum = ExpressiveText.CalcChecksum();

	FExpressiveText GenExprText = ExpressiveText.GetExpressiveText();
	GenExprText.DefineWorldContext(this);
	SetExpressiveText(GenExprText);
}

bool UExprTextBlock::IsCacheInvalid() const
{
	return CachedChecksum != ExpressiveText.CalcChecksum()
		|| !CachedSize.Equals(CurrentSize, 0.001f);
}

void UExprTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	CurrentSize = GetCachedGeometry().GetLocalSize();
	UpdateText();
}
