// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataGraphBase.h"
#include "AssetDefinitionDefault.h"
#include "DataGraphAssetEditor.h"
#include "DataGraphAssetDefinition.generated.h"

UCLASS()
class DATAGRAPHED_API UDataGraphAssetDefinition : public UAssetDefinitionDefault
{
	GENERATED_BODY()

public:
	
	UDataGraphAssetDefinition() {}
	virtual FLinearColor GetAssetColor() const override { return FColor(100, 160, 80); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UDataGraphBase::StaticClass(); }
	virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("DataGraphEditor", "DataGraph", "Data Graph"); }
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override
	{
		static const TArray Categories{FAssetCategoryPath(INVTEXT("Data Assets"))};
		return Categories;
	}

	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override
	{
		const EToolkitMode::Type Mode = OpenArgs.ToolkitHost.IsValid() ?
			EToolkitMode::WorldCentric : EToolkitMode::Standalone;

		for (UObject* Object : OpenArgs.LoadObjects<UObject>())
		{
			if (UDataGraphBase* Graph = Cast<UDataGraphBase>(Object))
			{
				const TSharedRef<FDataGraphAssetEditor> GraphEditor(new FDataGraphAssetEditor());
				GraphEditor->InitEditor(Mode, OpenArgs.ToolkitHost, Graph);
			}
		}

		return EAssetCommandResult::Handled;
	}
};
