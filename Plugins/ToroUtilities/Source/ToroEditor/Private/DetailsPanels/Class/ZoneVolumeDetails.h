// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "DetailWidgetRow.h"
#include "DetailsPanels/ToroActorDetails.h"
#include "ZoneSystem/ZoneVolume.h"

#define CLASSNAME AZoneVolume
class TOROEDITOR_API FZoneVolumeDetails final : public FToroActorDetails
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FZoneVolumeDetails>();
	}

private:
	
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		FToroActorDetails::CustomizeDetails(DetailBuilder);
		IDetailCategoryBuilder& Settings = DetailBuilder.EditCategory(TEXT("Settings"));
		Settings.AddProperty(CLASS_PROPERTY_NS(ZoneID));
		Settings.AddProperty(CLASS_PROPERTY_NS(EnterActions));
		Settings.AddProperty(CLASS_PROPERTY_NS(ExitActions));

		IDetailGroup& SmartCulling = Settings.AddGroup(TEXT("SmartCulling"), INVTEXT("Smart Culling"));
		{
			SmartCulling.AddPropertyRow(CLASS_PROPERTY_NS(CullInvert));
			SmartCulling.AddPropertyRow(CLASS_PROPERTY_NS(CullTargets));

			TArray<TWeakObjectPtr<UObject>> Objs;
			DetailBuilder.GetObjectsBeingCustomized(Objs);
			AZoneVolume* Zone = Objs.Num() == 1 ? Cast<AZoneVolume>(Objs[0]) : nullptr;

			CLASS_PROPERTY_VAR_NS(CullFindBounded, WithinBounds)
			CLASS_PROPERTY_VAR_NS(CullFindTag, FindTag)
			SmartCulling.AddWidgetRow().IsEnabled(IsValid(Zone))
			.NameContent()
			[
				SNew(SButton)
				.Text(IsValid(Zone) ? INVTEXT("Find Actors") : INVTEXT("Multiple"))
				.OnClicked_Lambda([Zone]()
				{
					if (Zone) Zone->FindCullTargets();
					return FReply::Handled();
				})
			]
			.ValueContent()
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.Padding(0.0f,  2.0f)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.MinWidth(50.0f)
					.AutoWidth()
					[
						WithinBounds->CreatePropertyNameWidget()
					]
					+SHorizontalBox::Slot()
					.MinWidth(75.0f)
					.AutoWidth()
					[
						WithinBounds->CreatePropertyValueWidget()
					]
				]
				+SVerticalBox::Slot()
				.Padding(0.0f,  2.0f)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.MinWidth(50.0f)
					.AutoWidth()
					[
						FindTag->CreatePropertyNameWidget()
					]
					+SHorizontalBox::Slot()
					.MinWidth(75.0f)
					.AutoWidth()
					[
						FindTag->CreatePropertyValueWidget()
					]
				]
			];
		}
	}
};
#undef CLASSNAME