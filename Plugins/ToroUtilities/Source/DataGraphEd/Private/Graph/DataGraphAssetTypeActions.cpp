// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataGraphAssetTypeActions.h"
#include "DataGraphAssetEditor.h"

uint32 FDataGraphAssetTypeActions::GetCategories()
{
	if (const FAssetToolsModule* AssetToolsModule = FModuleManager::LoadModulePtr<FAssetToolsModule>("AssetTools"))
	{
		return AssetToolsModule->Get().RegisterAdvancedAssetCategory(TEXT("CustomTypes"),
			NSLOCTEXT("AssetCategory", "CustomAssetCategory", "Custom Assets"));
	}

	return EAssetTypeCategories::Misc;
}

void FDataGraphAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ?
		EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	
	for (UObject* Object : InObjects)
	{
		if (UDataGraphBase* Graph = Cast<UDataGraphBase>(Object))
		{
			const TSharedRef<FDataGraphAssetEditor> GraphEditor(new FDataGraphAssetEditor());
			GraphEditor->InitEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}
