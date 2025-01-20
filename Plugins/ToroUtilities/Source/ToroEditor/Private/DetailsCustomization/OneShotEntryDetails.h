// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsCustomization/DetailsHelpers.h"
#include "DataTypes/OneShotDataTypes.h"

#define STRUCTNAME FOneShotEntry

FSlateFontInfo GetLoopingLabelFont()
{
	FSlateFontInfo Info = IDetailLayoutBuilder::GetDetailFontBold();
	Info.Size = 12;
	return Info;
}

class FOneShotEntryCustomization final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FOneShotEntryCustomization>();
	}

private:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		STRUCT_PROPERTY_VAR(Sound, Sound);
		StructBuilder.AddCustomRow(INVTEXT("Sound"))
		.NameContent() [ Sound->CreatePropertyNameWidget() ]
		.ValueContent()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.Padding(0.0f,  2.0f)
			.AutoHeight()
			[
				Sound->CreatePropertyValueWidget()
			]
			+SVerticalBox::Slot()
			.Padding(0.0f,  2.0f)
			.AutoHeight()
			[
				SNew(STextBlock) 
				.Text(INVTEXT("Looping Track"))
				.Font(GetLoopingLabelFont())
				.Visibility_Lambda([Sound]()
				{
					UObject* Object;
					Sound->GetValue(Object);
					const USoundBase* SoundObj = Cast<USoundBase>(Object);
					return SoundObj && SoundObj->IsLooping()
						? EVisibility::Visible : EVisibility::Collapsed;
				})
			]
		];

		STRUCT_PROPERTY_VAR(Fades, FadeTimes);
		StructBuilder.AddCustomRow(INVTEXT("Fade"))
		.NameContent() [ GENERIC_PROPERTY_LABEL(Fading) ]
		.ValueContent()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.Padding(0.0f,  2.0f)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.MinWidth(40.0f)
				.AutoWidth()
				[
					GENERIC_PROPERTY_LABEL(Play)
				]
				+SHorizontalBox::Slot()
				.MinWidth(75.0f)
				.AutoWidth()
				[
					FadeTimes->GetChildHandle(0)->CreatePropertyValueWidget()
				]
			]
			+SVerticalBox::Slot()
			.Padding(0.0f,  2.0f)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.MinWidth(40.0f)
				.AutoWidth()
				[
					GENERIC_PROPERTY_LABEL(Stop)
				]
				+SHorizontalBox::Slot()
				.MinWidth(75.0f)
				.AutoWidth()
				[
					FadeTimes->GetChildHandle(1)->CreatePropertyValueWidget()
				]
			]
			+SVerticalBox::Slot()
			.Padding(0.0f,  2.0f)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.MinWidth(40.0f)
				.AutoWidth()
				[
					GENERIC_PROPERTY_LABEL(Pause)
				]
				+SHorizontalBox::Slot()
				.MinWidth(75.0f)
				.AutoWidth()
				[
					FadeTimes->GetChildHandle(2)->CreatePropertyValueWidget()
				]
			]
		];

		StructBuilder.AddProperty(STRUCT_PROPERTY(Volume));
		StructBuilder.AddProperty(STRUCT_PROPERTY(StartRange));
	}
};
#undef STRUCTNAME