// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "DetailsCustomization/ToroActorDetails.h"
#include "PlayerChar/ToroFirstPersonPlayer.h"

#define CLASSNAME AToroFirstPersonPlayer
class TOROEDITOR_API FFirstPersonPlayerCustomization final : public FToroActorCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FFirstPersonPlayerCustomization>();
	}

private:
	
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		FToroActorCustomization::CustomizeDetails(DetailBuilder);
	}
};
#undef CLASSNAME