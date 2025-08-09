// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditor.h"
#include "GameHints/HintDatabase.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "HintDBAsset.generated.h"

UCLASS()
class TOROEDITOR_API UHintDBFactory final : public UToroAssetFactory
{
	GENERATED_BODY()

public:

	UHintDBFactory()
	{
		AssetName = TEXT("HintDatabase");
		SupportedClass = UHintDatabase::StaticClass();
	}
};

UCLASS()
class TOROEDITOR_API UHintDBDefinition final : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	virtual FLinearColor GetAssetColor() const override { return FColor(20, 160, 170); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UHintDatabase::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "HintDatabaseName", "Hint Database");
	}

	// virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override
	// {
	// 	return FToroEditorStyle::Get().GetBrush("ClassThumbnail.HintDatabase");
	// }
};
