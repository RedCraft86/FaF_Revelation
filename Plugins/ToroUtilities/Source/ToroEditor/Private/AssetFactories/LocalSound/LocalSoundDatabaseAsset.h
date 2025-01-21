// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/LocalSoundTypes.h"
#include "AssetFactories/ToroAssetTypeActions.h"

class FLocalSoundDatabaseAsset final : public FToroAssetTypeActions
{
public:

	explicit FLocalSoundDatabaseAsset() {}
	virtual FColor GetTypeColor() const override { return FColor(160, 25, 100); }
	virtual UClass* GetSupportedClass() const override { return ULocalSoundDatabase::StaticClass(); }
	virtual FText GetName() const override { return NSLOCTEXT("ToroEditor", "LocalSoundDatabaseName", "Local Sound Database"); }
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override
	{
		const ULocalSoundDatabase* DB = Cast<ULocalSoundDatabase>(AssetData.GetAsset());
		const int32 All = DB->LocalSounds.Num(), Valid = DB->GetValidCount();
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