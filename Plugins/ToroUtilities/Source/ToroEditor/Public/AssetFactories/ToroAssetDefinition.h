// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AssetTagData.h"
#include "ToroDatabase.h"
#include "AssetDefinitionDefault.h"
#include "ToroAssetDefinition.generated.h"

UCLASS(Abstract)
class TOROEDITOR_API UToroAssetDefinition : public UAssetDefinitionDefault
{
	GENERATED_BODY()

public:
	
	UToroAssetDefinition() {}

	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override
	{
		return {FAssetCategoryPath(NSLOCTEXT("AssetCategory", "CustomAssetCategory", "Custom Assets"))};
	}

	virtual FText GetAssetDescription(const FAssetData& AssetData) const override
	{
		FText UserDataText = FText::GetEmpty();
		if (IInterface_AssetUserData* UserData = Cast<IInterface_AssetUserData>(AssetData.GetAsset()))
		{
			if (const UAssetTagData* TagData = UserData->GetAssetUserData<UAssetTagData>())
			{
				const TSet<FString> Tags = TagData->Tags;
				UserDataText = Tags.IsEmpty() ? FText::GetEmpty()
					: FText::FromString(FString::Join(Tags, TEXT(", ")));
			}
		}

		FText DatabaseText = FText::GetEmpty();
		if (const UToroDatabase* DB = Cast<UToroDatabase>(AssetData.GetAsset()))
		{
			const int32 All = DB->GetTotalData(), Valid = DB->GetValidData();
			DatabaseText = FText::Format(INVTEXT("All: {0}\nValid: {1}\nInvalid: {2}"), All, Valid, All - Valid);
			
		}

		FText OutText = FText::GetEmpty();
		if (const FText SuperText = Super::GetAssetDescription(AssetData); !SuperText.IsEmptyOrWhitespace())
		{
			OutText = FText::Format(INVTEXT("{0}\n{1}\n{2}"), DatabaseText, SuperText, UserDataText);
		}

		return OutText;
	}
};
