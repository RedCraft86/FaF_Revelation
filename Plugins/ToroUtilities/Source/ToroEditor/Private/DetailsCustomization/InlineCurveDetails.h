// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsCustomization/DetailsHelpers.h"
#include "InlineCurves.h"

class FInlineCurveCustomization final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FInlineCurveCustomization>();
	}

private:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
		{
			StructBuilder.AddProperty(STRUCT_CLASS_PROPERTY(FInlineFloatCurve, Curve))
				.DisplayName(StructPropertyHandle->GetPropertyDisplayName());
		}
};