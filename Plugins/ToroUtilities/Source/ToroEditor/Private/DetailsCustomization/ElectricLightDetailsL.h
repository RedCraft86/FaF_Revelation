// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "ElectricSystem/ElectricLightBaseL.h"
#include "DetailsCustomization/ElectricActorDetails.h"

#define CLASSNAME AElectricLightBaseL
class TOROEDITOR_API FElectricLightCustomization final : public FElectricActorCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FElectricLightCustomization>();
	}

private:
	
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		FElectricActorCustomization::CustomizeDetails(DetailBuilder);
		
		IDetailCategoryBuilder& Settings = DetailBuilder.EditCategory(TEXT("Settings"));
		IDetailGroup& FlickerGroup = Settings.AddGroup(TEXT("Flicker"), INVTEXT("Flicker"));
		{
			FlickerGroup.HeaderProperty(GET_PROPERTY(bFlicker));
			FlickerGroup.AddPropertyRow(GET_PROPERTY(FlickerRate));
			FlickerGroup.AddPropertyRow(GET_PROPERTY(FlickerRange));
			FlickerGroup.AddPropertyRow(GET_PROPERTY(FlickerPlayback));
			FlickerGroup.AddPropertyRow(GET_PROPERTY(FlickerCurve));
		}
	}
};
#undef CLASSNAME