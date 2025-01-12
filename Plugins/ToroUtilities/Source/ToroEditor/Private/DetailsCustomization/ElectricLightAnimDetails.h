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
		StructBuilder.AddProperty(STRUCT_PROPERTY(AnimTime));

		STRUCT_PROPERTY_VAR(LightRange, LightRange);
		STRUCT_PROPERTY_VAR(bLightRange, bLightRange);
		StructBuilder.AddCustomRow(INVTEXT("Light Range"))
		.NameContent()
		[
			LightRange->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.Padding(0.0f, 0.0f, 5.0f, 0.0f)
			.AutoWidth()
			[
				bLightRange->CreatePropertyValueWidget()
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.MinWidth(75.0f)
				.AutoWidth()
				[
					LightRange->GetChildHandle(0)->CreatePropertyValueWidget()
				]
				+SHorizontalBox::Slot()
				.MinWidth(75.0f)
				.AutoWidth()
				[
					LightRange->GetChildHandle(1)->CreatePropertyValueWidget()
				]
			]
		];

		STRUCT_PROPERTY_VAR(MeshRange, MeshRange);
		STRUCT_PROPERTY_VAR(bMeshRange, bMeshRange);
		StructBuilder.AddCustomRow(INVTEXT("Mesh Range"))
		.NameContent()
		[
			MeshRange->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.Padding(0.0f, 0.0f, 5.0f, 0.0f)
			.AutoWidth()
			[
				bMeshRange->CreatePropertyValueWidget()
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.MinWidth(75.0f)
				.AutoWidth()
				[
					MeshRange->GetChildHandle(0)->CreatePropertyValueWidget()
				]
				+SHorizontalBox::Slot()
				.MinWidth(75.0f)
				.AutoWidth()
				[
					MeshRange->GetChildHandle(1)->CreatePropertyValueWidget()
				]
			]
		];
		
		StructBuilder.AddProperty(STRUCT_PROPERTY(AnimCurve));
	}
};
#undef STRUCTNAME