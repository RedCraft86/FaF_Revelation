// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AssetTagData.h"
#include "ToroDataAsset.h"
#include "AssetDefinitionDefault.h"
#include "ToroAssetDefinition.generated.h"

UCLASS(Abstract)
class TOROEDITOR_API UToroAssetDefinition : public UAssetDefinitionDefault
{
	GENERATED_BODY()

public:

	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override
	{
		static const TArray Categories{EAssetCategoryPaths::Blueprint / INVTEXT("ToroUtilities")};
		return Categories;
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

		FText DataAssetText = FText::GetEmpty();
		if (const UToroDataAsset* DA = Cast<UToroDataAsset>(AssetData.GetAsset()))
		{
			DataAssetText = DA->GetDescription();
		}

		FText OutText = FText::GetEmpty();
		if (const FText SuperText = Super::GetAssetDescription(AssetData); !SuperText.IsEmptyOrWhitespace())
		{
			OutText = FText::Format(INVTEXT("{0}\n{1}\n{2}"), DataAssetText, SuperText, UserDataText);
		}

		return OutText;
	}
};
