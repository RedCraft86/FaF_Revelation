// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsPanels/CustomizationHelpers.h"
#include "DataTypes/InlineCurves.h"

class FInlineCurveDetails final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FInlineCurveDetails>();
	}

private:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		StructBuilder.AddProperty(STRUCT_PROPERTY(FInlineFloatCurve, Curve))
			.DisplayName(StructPropertyHandle->GetPropertyDisplayName());
	}
};