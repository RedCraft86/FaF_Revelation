// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditor.h"
#include "Tutorial/TutorialDatabase.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "TutorialDBAsset.generated.h"

UCLASS()
class TOROEDITOR_API UTutorialDBFactory final : public UToroAssetFactory
{
	GENERATED_BODY()

public:

	UTutorialDBFactory()
	{
		AssetName = TEXT("TutorialDatabase");
		SupportedClass = UTutorialDatabase::StaticClass();
	}
};

UCLASS()
class TOROEDITOR_API UTutorialDBDefinition final : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	virtual FLinearColor GetAssetColor() const override { return FColor(20, 150, 120); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UTutorialDatabase::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "TutorialDatabaseName", "Tutorial Database");
	}

	virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override
	{
		return FToroEditorStyle::Get().GetBrush("ClassThumbnail.TutorialDatabase");
	}
};
