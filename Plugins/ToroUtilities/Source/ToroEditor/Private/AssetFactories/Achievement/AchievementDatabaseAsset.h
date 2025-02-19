// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Achievements/AchievementTypes.h"
#include "AssetFactories/ToroAssetTypeActions.h"

class FAchievementDatabaseAsset final : public FToroAssetTypeActions
{
public:

	explicit FAchievementDatabaseAsset() {}
	virtual FColor GetTypeColor() const override { return FColor(200, 200, 10); }
	virtual UClass* GetSupportedClass() const override { return UAchievementDatabase::StaticClass(); }
	virtual FText GetName() const override { return NSLOCTEXT("ToroEditor", "AchievementDatabaseName", "Achievement Database"); }
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override
	{
		const UAchievementDatabase* DB = Cast<UAchievementDatabase>(AssetData.GetAsset());
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