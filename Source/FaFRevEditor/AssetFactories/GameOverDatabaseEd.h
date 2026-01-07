// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ToroEditor.h"
#include "GameOver/GameOverDatabase.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "GameOverDatabaseEd.generated.h"

UCLASS()
class FAFREVEDITOR_API UGameOverDatabaseFactory final : public UToroAssetFactory
{
	GENERATED_BODY()

public:

	UGameOverDatabaseFactory()
	{
		AssetName = TEXT("GameOverDatabase");
		SupportedClass = UGameOverDatabase::StaticClass();
	}
};

UCLASS()
class FAFREVEDITOR_API UGameOverDatabaseDefinition final : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	virtual FLinearColor GetAssetColor() const override { return FColor(180, 25, 50); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UGameOverDatabase::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "GameOverDatabaseName", "GameOver Database");
	}

	virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override
	{
		return FToroEditorStyle::Get().GetBrush("ClassThumbnail.GameOverDatabase");
	}
};