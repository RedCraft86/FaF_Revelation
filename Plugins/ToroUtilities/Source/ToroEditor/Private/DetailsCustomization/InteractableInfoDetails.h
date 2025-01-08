﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailWidgetRow.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsCustomization/DetailsHelpers.h"
#include "Interaction/InteractionInterface.h"

#define STRUCTNAME FInteractableInfo

class FInteractableInfoCustomization final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FInteractableInfoCustomization>();
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
		StructBuilder.AddProperty(STRUCT_PROPERTY(HoldingTime).ToSharedRef());
		
		IDetailGroup& LabelGroup = StructBuilder.AddGroup(TEXT("Label"), INVTEXT("Label"));
		{
			LabelGroup.HeaderProperty(STRUCT_PROPERTY(Label).ToSharedRef());
			LabelGroup.AddPropertyRow(STRUCT_PROPERTY(LabelOffset).ToSharedRef());
		}
		
		IDetailGroup& IconGroup = StructBuilder.AddGroup(TEXT("Icon"), INVTEXT("Icon"));
		{
			IconGroup.HeaderProperty(STRUCT_PROPERTY(Icon).ToSharedRef());
			IconGroup.AddPropertyRow(STRUCT_PROPERTY(IconSize).ToSharedRef());
		}
	}
};
#undef STRUCTNAME