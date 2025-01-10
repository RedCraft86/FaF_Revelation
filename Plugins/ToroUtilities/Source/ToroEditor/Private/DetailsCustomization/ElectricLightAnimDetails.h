// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailWidgetRow.h"
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
		STRUCT_PROPERTY_VAR(bEnabled, bEnabled)
		HeaderRow.NameContent() [ StructPropertyHandle->CreatePropertyNameWidget() ]
		.ValueContent()
		[
			bEnabled->CreatePropertyValueWidget()
		];
	}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		STRUCT_PROPERTY_VAR(PlayRate, PlayRate);
		StructBuilder.AddProperty(PlayRate.ToSharedRef());
		
		STRUCT_PROPERTY_VAR(IntensityRange, IntensityRange);
		StructBuilder.AddProperty(IntensityRange.ToSharedRef());
		
		STRUCT_PROPERTY_VAR(AnimCurve, AnimCurve);
		StructBuilder.AddProperty(AnimCurve.ToSharedRef());
	}
};
#undef STRUCTNAME