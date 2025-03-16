// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AssetTypeActions_Base.h"
#include "DataGraphBase.h"

class FDataGraphAssetTypeActions final : public FAssetTypeActions_Base
{
public:
	
	explicit FDataGraphAssetTypeActions() {}
	
	virtual uint32 GetCategories() override;
	virtual FColor GetTypeColor() const override { return FColor(100, 160, 80); }
	virtual UClass* GetSupportedClass() const override { return UDataGraphBase::StaticClass(); }
	virtual FText GetName() const override { return NSLOCTEXT("DataGraphEditor", "GenericName", "Data Graph"); }
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
};
