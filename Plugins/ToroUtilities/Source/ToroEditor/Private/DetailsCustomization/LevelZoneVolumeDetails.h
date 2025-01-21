// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "DetailsCustomization/ToroActorDetails.h"
#include "Misc/LevelZoneVolume.h"

#define CLASSNAME ALevelZoneVolume
class TOROEDITOR_API FLevelZoneVolumeCustomization final : public FToroActorCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FLevelZoneVolumeCustomization>();
	}

private:
	
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		FToroActorCustomization::CustomizeDetails(DetailBuilder);
		IDetailCategoryBuilder& Settings = DetailBuilder.EditCategory(TEXT("Settings"));
		Settings.AddProperty(GET_PROPERTY(ZoneID));

		IDetailGroup& SmartCulling = Settings.AddGroup(TEXT("SmartCulling"), INVTEXT("Smart Culling"));
		{
			SmartCulling.AddPropertyRow(GET_PROPERTY(CullInvert));
			SmartCulling.AddPropertyRow(GET_PROPERTY(CullTargets));

			TArray<TWeakObjectPtr<UObject>> Objs;
			DetailBuilder.GetObjectsBeingCustomized(Objs);
			ALevelZoneVolume* Zone = Objs.Num() == 1 ? Cast<ALevelZoneVolume>(Objs[0]) : nullptr;

			GET_PROPERTY_VAR(CullFindBounded, WithinBounds)
			GET_PROPERTY_VAR(CullFindTag, FindTag)
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

		IDetailGroup& LocalSound = Settings.AddGroup(TEXT("LocalSound"), INVTEXT("LocalSound"));
		{
			LocalSound.HeaderProperty(GET_PROPERTY(LocalSoundTag)).DisplayName(INVTEXT("Local Sound"));
			LocalSound.AddPropertyRow(GET_PROPERTY(LocalSoundPlayOnce));
			LocalSound.AddPropertyRow(GET_PROPERTY(LocalSoundCooldown));
		}
	}
};
#undef CLASSNAME