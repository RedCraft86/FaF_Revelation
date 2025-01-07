// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "WorldMusic/WorldMusicVolume.h"
#include "DetailsCustomization/ToroActorDetails.h"

#define CLASSNAME AWorldMusicVolume
class TOROEDITOR_API FWorldMusicVolumeCustomization final : public FToroActorCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FWorldMusicVolumeCustomization>();
	}

private:
	
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		FToroActorCustomization::CustomizeDetails(DetailBuilder);
		
		IDetailCategoryBuilder& Settings = DetailBuilder.EditCategory(TEXT("Settings"));
		Settings.AddProperty(GET_PROPERTY(bStopOnExit));
		Settings.AddProperty(GET_PROPERTY(Cooldown));

		IDetailGroup& SoundGroup = Settings.AddGroup(TEXT("Sound"), INVTEXT("Sound"));
		{
			SoundGroup.HeaderProperty(GET_PROPERTY(Sound));
			SoundGroup.AddPropertyRow(GET_PROPERTY(Volume));
			SoundGroup.AddPropertyRow(GET_PROPERTY(FadeTime));
			SoundGroup.AddPropertyRow(GET_PROPERTY(StartRange));
		}
	}
};
#undef CLASSNAME