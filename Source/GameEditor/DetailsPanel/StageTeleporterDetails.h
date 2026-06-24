// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "DetailsPanel/ToroStructCustomization.h"
#include "GameStage/GameStageNode.h"

#define STRUCT_NAME FStageTeleporter
class FStageTeleporterDetails final : public FToroStructCustomization
{
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle, FDetailWidgetRow& HeaderRow, 
		IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		GET_STRUCT_PROPERTY_VAR_NS(Target, Target);
		HeaderRow.NameContent()
		[
			StructHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.HAlign(HAlign_Fill)
		[
			Target->CreatePropertyValueWidget()
		];
	}

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		GET_STRUCT_PROPERTY_VAR_NS(bContinuous, Continuous);
		StructBuilder.AddProperty(Continuous.ToSharedRef());
	}
};
#undef STRUCT_NAME