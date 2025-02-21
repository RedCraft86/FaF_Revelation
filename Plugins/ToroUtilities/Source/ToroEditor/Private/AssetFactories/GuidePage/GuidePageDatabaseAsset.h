// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GuidePages/GuidePageTypes.h"
#include "AssetFactories/ToroAssetTypeActions.h"

class FGuidePageDatabaseAsset final : public FToroAssetTypeActions
{
public:

	explicit FGuidePageDatabaseAsset() {}
	virtual FColor GetTypeColor() const override { return FColor(20, 150, 120); }
	virtual UClass* GetSupportedClass() const override { return UGuidePageDatabase::StaticClass(); }
	virtual FText GetName() const override { return NSLOCTEXT("ToroEditor", "GuidePageDatabaseName", "Local Sound Database"); }
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override
	{
		const UGuidePageDatabase* DB = Cast<UGuidePageDatabase>(AssetData.GetAsset());
		const int32 All = DB->Entries.Num(), Valid = DB->GetValidCount();
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