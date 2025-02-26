// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/FootstepTypes.h"
#include "AssetFactories/ToroAssetTypeActions.h"

class FFootstepDatabaseAsset final : public FToroAssetTypeActions
{
public:

	explicit FFootstepDatabaseAsset() {}
	virtual FColor GetTypeColor() const override { return FColor(150, 120, 100); }
	virtual UClass* GetSupportedClass() const override { return UFootstepDatabase::StaticClass(); }
	virtual FText GetName() const override { return NSLOCTEXT("ToroEditor", "FootstepDatabaseName", "Footstep Database"); }
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override
	{
		const UFootstepDatabase* DB = Cast<UFootstepDatabase>(AssetData.GetAsset());
		const int32 All = DB->AllEntries(), Valid = DB->ValidEntries();
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