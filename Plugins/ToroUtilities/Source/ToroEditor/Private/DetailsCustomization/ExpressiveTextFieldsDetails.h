// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsCustomization/DetailsHelpers.h"
#include "Handles/ExpressiveText.h"

#define STRUCTNAME FExpressiveTextFields
class FExpressiveTextFieldsCustomization final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FExpressiveTextFieldsCustomization>();
	}

private:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		STRUCT_PROPERTY_VAR(Text, Text);
		HeaderRow.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			Text->CreatePropertyValueWidget()
		];
	}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		StructBuilder.AddProperty(STRUCT_PROPERTY(DefaultStyle));
		StructBuilder.AddProperty(STRUCT_PROPERTY(Justification));

		STRUCT_PROPERTY_VAR(UseAutoSize, UseAutoSize);
		IDetailGroup& AutoSizeGroup = StructBuilder.AddGroup(TEXT("AutoSize"), INVTEXT("Auto Size"));
		{
			AutoSizeGroup.HeaderProperty(UseAutoSize.ToSharedRef()).DisplayName(INVTEXT("Auto Size"));

			STRUCT_PROPERTY_VAR(DefaultFontSize, DefaultFontSize);
			STRUCT_PROPERTY_VAR(UseDefaultFontSize, UseDefaultFontSize);
			auto DefaultFontSizeWidget = [DefaultFontSize, UseDefaultFontSize]()
			{
				TSharedRef<SWidget> Widget = DefaultFontSize->CreatePropertyValueWidget();
				Widget->SetEnabled(TAttribute<bool>::CreateLambda([UseDefaultFontSize]()
				{
					bool bUseFontSize = false;
					UseDefaultFontSize->GetValue(bUseFontSize);
					return bUseFontSize;
				}));
				return Widget;
			};
		
			AutoSizeGroup.AddWidgetRow()
			.Visibility(TAttribute<EVisibility>::CreateLambda([UseAutoSize]()
			{
				bool bAutoSize = false;
				UseAutoSize->GetValue(bAutoSize);
				return bAutoSize ? EVisibility::Collapsed : EVisibility::Visible;
			}))
			.NameContent()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 5.0f, 0.0f)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					UseDefaultFontSize->CreatePropertyValueWidget()
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					GENERIC_PROPERTY_LABEL(Custom Size)
				]
			]
			.ValueContent()
			[
				DefaultFontSizeWidget()
			];
		}

		STRUCT_PROPERTY_VAR(WrapSettings, WrapSettings);
		IDetailGroup& WrapGroup = StructBuilder.AddGroup(TEXT("WrapSettings"), INVTEXT("Wrapping"));
		{
			const TSharedPtr<IPropertyHandle> WrapMode = WrapSettings->GetChildHandle(0);
			WrapGroup.HeaderProperty(WrapMode.ToSharedRef()).DisplayName(INVTEXT("Wrapping"));

			WrapGroup.AddPropertyRow(WrapSettings->GetChildHandle(1).ToSharedRef())
				.Visibility(TAttribute<EVisibility>::CreateLambda([WrapMode]()
				{
					uint8 WrapModeVal = 0;
					WrapMode->GetValue(WrapModeVal);
					return WrapModeVal < 2 ? EVisibility::Visible : EVisibility::Collapsed;
				}
			));
		}

		STRUCT_PROPERTY_VAR(Alignment, Alignment);
		IDetailGroup& AlignmentGroup = StructBuilder.AddGroup(TEXT("AlignmentSettings"), INVTEXT("Alignment"));
		{
			constexpr float Width(75.0f);
			
			AlignmentGroup.HeaderRow()
			.NameContent()
			[
				Alignment->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 5.0f, 0.0f)
				.VAlign(VAlign_Center)
				.MinWidth(Width)
				.AutoWidth()
				[
					GENERIC_PROPERTY_LABEL(Vertical).Justification(ETextJustify::Center)
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.MinWidth(Width)
				.AutoWidth()
				[
					GENERIC_PROPERTY_LABEL(Horizontal).Justification(ETextJustify::Center)
				]
			];

			AlignmentGroup.AddWidgetRow()
			.NameContent()
			[
				GENERIC_PROPERTY_LABEL(Position)
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 5.0f, 0.0f)
				.VAlign(VAlign_Center)
				.MinWidth(Width)
				.AutoWidth()
				[
					Alignment->GetChildHandle(0)->CreatePropertyValueWidget()
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.MinWidth(Width)
				.AutoWidth()
				[
					Alignment->GetChildHandle(1)->CreatePropertyValueWidget()
				]
			];

			AlignmentGroup.AddWidgetRow()
			.NameContent()
			[
				GENERIC_PROPERTY_LABEL(Offset Type)
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 5.0f, 0.0f)
				.MaxWidth(Width).MinWidth(Width)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					Alignment->GetChildHandle(4)->CreatePropertyValueWidget()
				]
				+SHorizontalBox::Slot()
				.MaxWidth(Width).MinWidth(Width)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					Alignment->GetChildHandle(5)->CreatePropertyValueWidget()
				]
			];

			AlignmentGroup.AddWidgetRow()
			.NameContent()
			[
				GENERIC_PROPERTY_LABEL(Offset)
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 5.0f, 0.0f)
				.VAlign(VAlign_Center)
				.MinWidth(Width)
				.AutoWidth()
				[
					Alignment->GetChildHandle(2)->CreatePropertyValueWidget()
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.MinWidth(Width)
				.AutoWidth()
				[
					Alignment->GetChildHandle(3)->CreatePropertyValueWidget()
				]
			];
		}

		StructBuilder.AddProperty(STRUCT_PROPERTY(RefreshAutomaticallyOnChange)).DisplayName(INVTEXT("Update On Change"));
		StructBuilder.AddProperty(STRUCT_PROPERTY(Actions));
		StructBuilder.AddProperty(STRUCT_PROPERTY(ReferencedResources));
	}
};
#undef STRUCTNAME