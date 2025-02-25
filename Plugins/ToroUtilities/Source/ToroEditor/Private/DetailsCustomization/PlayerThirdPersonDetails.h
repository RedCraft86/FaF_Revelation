// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "DetailsCustomization/ToroActorDetails.h"
#include "PlayerChar/ToroThirdPersonPlayer.h"

#define CLASSNAME AToroThirdPersonPlayer
class TOROEDITOR_API FThirdPersonPlayerCustomization final : public FToroActorCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FThirdPersonPlayerCustomization>();
	}

private:
	
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		FToroActorCustomization::CustomizeDetails(DetailBuilder);
	}
};
#undef CLASSNAME