// Copyright (C) RedCraft86. All Rights Reserved.

// ReSharper disable CppPassValueParameterByConstReference
#pragma once

#include "IPropertyTypeCustomization.h"
#include "DetailsCustomization/DetailsHelpers.h"
#include "DataTypes/GenericPlayerTypes.h"

#define STRUCTNAME FPlayerFootsteps
class FPlayerFootstepCustomization final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FPlayerFootstepCustomization>();
	}

private:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		IDetailGroup& Footsteps = StructBuilder.AddGroup(TEXT("Footsteps"), StructPropertyHandle->GetPropertyDisplayName());
		{
			Footsteps.AddPropertyRow(STRUCT_PROPERTY(Volume));
			Footsteps.AddPropertyRow(STRUCT_PROPERTY(FloorTrace));

			STRUCT_PROPERTY_VAR(Intervals, Intervals)
			
			TSharedPtr<IPropertyHandle> IntWalk = Intervals->GetChildHandle(0);
			ADD_CLAMP_META_NO_UPPER(IntWalk, 0.1, 1.0)

			TSharedPtr<IPropertyHandle> IntRun = Intervals->GetChildHandle(1);
			ADD_CLAMP_META_NO_UPPER(IntRun, 0.1, 1.0)
			
			TSharedPtr<IPropertyHandle> IntCrouch = Intervals->GetChildHandle(2);
			ADD_CLAMP_META_NO_UPPER(IntCrouch, 0.1, 1.0)
			
			Footsteps.AddWidgetRow().OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(Intervals))
			.NameContent()
			[
				Intervals->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.Padding(0.0f, 2.0f, 0.0f, 2.0f)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.Padding(0.0f, 0.0f, 12.0f, 0.0f)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						GENERIC_PROPERTY_LABEL(Walk)
					]
					+SHorizontalBox::Slot()
					.MinWidth(90.0f)
					.AutoWidth()
					[
						IntWalk->CreatePropertyValueWidget()
					]
				]
				+SVerticalBox::Slot()
				.Padding(0.0f, 2.0f, 0.0f, 2.0f)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.Padding(0.0f, 0.0f, 17.0f, 0.0f)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						GENERIC_PROPERTY_LABEL(Run)
					]
					+SHorizontalBox::Slot()
					.MinWidth(90.0f)
					.AutoWidth()
					[
						IntRun->CreatePropertyValueWidget()
					]
				]
				+SVerticalBox::Slot()
				.Padding(0.0f, 2.0f, 0.0f, 2.0f)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.Padding(0.0f, 0.0f, 2.0f, 0.0f)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						GENERIC_PROPERTY_LABEL(Crouch)
					]
					+SHorizontalBox::Slot()
					.MinWidth(90.0f)
					.AutoWidth()
					[
						IntCrouch->CreatePropertyValueWidget()
					]
				]
			];

			Footsteps.AddPropertyRow(STRUCT_PROPERTY(Sounds));
		}
	}
};
#undef STRUCTNAME