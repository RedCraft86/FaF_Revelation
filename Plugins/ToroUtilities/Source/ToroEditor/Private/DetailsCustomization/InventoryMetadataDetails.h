// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsCustomization/DetailsHelpers.h"
#include "Inventory/InventoryItemData.h"

class FInventoryMetadataCustomization final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FInventoryMetadataCustomization>();
	}

private:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		StructBuilder.AddProperty(STRUCT_CLASS_PROPERTY(FInventoryMetadata, Metadata))
			.DisplayName(StructPropertyHandle->GetPropertyDisplayName());
	}
};
