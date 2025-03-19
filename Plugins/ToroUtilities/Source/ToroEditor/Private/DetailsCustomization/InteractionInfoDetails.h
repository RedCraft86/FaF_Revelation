// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsCustomization/DetailsHelpers.h"
#include "Interaction/InteractionInterface.h"

#define STRUCTNAME FInteractionInfo
class FInteractionInfoCustomization final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FInteractionInfoCustomization>();
	}

private:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		STRUCT_PROPERTY_VAR(Interaction, Interaction)
		HeaderRow.NameContent() [ StructPropertyHandle->CreatePropertyNameWidget() ]
		.ValueContent()
		[
			Interaction->CreatePropertyValueWidget()
		];
	}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		StructBuilder.AddProperty(STRUCT_PROPERTY(HoldingTime));
		
		IDetailGroup& LabelGroup = StructBuilder.AddGroup(TEXT("Label"), INVTEXT("Label"));
		{
			LabelGroup.HeaderProperty(STRUCT_PROPERTY(Label));
			LabelGroup.AddPropertyRow(STRUCT_PROPERTY(LabelOffset));
		}
		
		IDetailGroup& IconGroup = StructBuilder.AddGroup(TEXT("Icon"), INVTEXT("Icon"));
		{
			IconGroup.HeaderProperty(STRUCT_PROPERTY(Icon));
			IconGroup.AddPropertyRow(STRUCT_PROPERTY(IconSize));
		}
	}
};
#undef STRUCTNAME