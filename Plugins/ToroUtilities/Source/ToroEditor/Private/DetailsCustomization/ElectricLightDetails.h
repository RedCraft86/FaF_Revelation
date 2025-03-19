// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "ElectricSystem/ElectricLightBase.h"
#include "DetailsCustomization/ElectricActorDetails.h"

#define CLASSNAME AElectricLightBase
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
		Settings.AddProperty(GET_PROPERTY(LightTickType));

		IDetailGroup& MeshGroup = Settings.AddGroup(TEXT("Mesh"), INVTEXT("Mesh"));
		{
			MeshGroup.AddPropertyRow(GET_PROPERTY(MeshMulti));
			MeshGroup.AddPropertyRow(GET_PROPERTY(MeshFresnel));
		}
	}
};
#undef CLASSNAME