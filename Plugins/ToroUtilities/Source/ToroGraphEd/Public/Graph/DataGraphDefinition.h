// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataGraphBase.h"
#include "DataGraphAssetEd.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "DataGraphDefinition.generated.h"

UCLASS()
class UDataGraphDefinition final : public UToroAssetDefinition
{
	GENERATED_BODY()

public:
	
	UDataGraphDefinition() {}
	virtual FLinearColor GetAssetColor() const override { return FColor(100, 160, 80); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UDataGraphBase::StaticClass(); }
	virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("ToroGraphEd", "DataGraph", "Data Graph"); }
	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override
	{
		const EToolkitMode::Type Mode = OpenArgs.ToolkitHost.IsValid() ?
			EToolkitMode::WorldCentric : EToolkitMode::Standalone;

		for (UObject* Object : OpenArgs.LoadObjects<UObject>())
		{
			if (UDataGraphBase* Graph = Cast<UDataGraphBase>(Object))
			{
				const TSharedRef<FDataGraphAssetEd> GraphEditor(new FDataGraphAssetEd());
				GraphEditor->InitEditor(Mode, OpenArgs.ToolkitHost, Graph);
			}
		}

		return EAssetCommandResult::Handled;
	}
};
