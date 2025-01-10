// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ElectricSystem/ElectricActorBase.h"
#include "DetailsCustomization/ToroActorDetails.h"

class TOROEDITOR_API FElectricActorCustomization : public FToroActorCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FElectricActorCustomization>();
	}

protected:
	
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		FToroActorCustomization::CustomizeDetails(DetailBuilder);
		IDetailCategoryBuilder& Settings = DetailBuilder.EditCategory(TEXT("Settings"));
		Settings.AddProperty(GET_CLASS_PROPERTY(AElectricActorBase, bPreviewState));
		Settings.AddProperty(GET_CLASS_PROPERTY(AElectricActorBase, MinEnergy));
		Settings.AddProperty(GET_CLASS_PROPERTY(AElectricActorBase, BreakStage));
	}
};
