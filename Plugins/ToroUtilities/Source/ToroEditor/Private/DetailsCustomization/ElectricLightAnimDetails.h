// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsCustomization/DetailsHelpers.h"
#include "ElectricSystem/ElectricLightBase.h"

#define STRUCTNAME FElectricLightAnim
class FElectricLightAnimCustomization final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FElectricLightAnimCustomization>();
	}

private:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		STRUCT_PROPERTY_VAR(bEnabled, bEnabled);
		HeaderRow.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			bEnabled->CreatePropertyValueWidget()
		];
	}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		StructBuilder.AddProperty(STRUCT_PROPERTY(PlayRate));
		StructBuilder.AddProperty(STRUCT_PROPERTY(LightRange));
		
		STRUCT_PROPERTY_VAR(bMeshRange, bMeshRange);
		STRUCT_PROPERTY_VAR(MeshRange, MeshRange);
		StructBuilder.AddCustomRow(INVTEXT("Mesh Range"))
		.NameContent()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.Padding(0.0f, 0.0f, 5.0f, 0.0f)
			.AutoWidth()
			[
				MeshRange->CreatePropertyNameWidget()
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				bMeshRange->CreatePropertyValueWidget()
			]
		]
		.ValueContent()
		[
			MeshRange->CreatePropertyValueWidget()
		];

		StructBuilder.AddProperty(STRUCT_PROPERTY(AnimTime));
		StructBuilder.AddProperty(STRUCT_PROPERTY(AnimCurve));
	}
};
#undef STRUCTNAME