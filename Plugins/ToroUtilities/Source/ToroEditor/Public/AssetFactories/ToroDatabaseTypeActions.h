// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroDatabase.h"
#include "AssetFactories/ToroAssetTypeActions.h"

class FToroDatabaseTypeActions : public FToroAssetTypeActions
{
public:

	explicit FToroDatabaseTypeActions() {}
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override
	{
		const UToroDatabase* DB = Cast<UToroDatabase>(AssetData.GetAsset());
		const int32 All = DB->GetTotalData(), Valid = DB->GetValidData();
		FText OutText = FText::Format(INVTEXT("All: {0}\nValid: {1}\nInvalid: {2}"),
			All, Valid, All - Valid);

		const FText SuperText = FAssetTypeActions_Base::GetAssetDescription(AssetData);
		if (!SuperText.IsEmptyOrWhitespace())
		{
			OutText = FText::Format(INVTEXT("{0}\n{2}"), OutText, SuperText);
		}

		return OutText;
	}
};