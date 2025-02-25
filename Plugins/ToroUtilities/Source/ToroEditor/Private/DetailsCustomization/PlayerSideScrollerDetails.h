// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "DetailsCustomization/ToroActorDetails.h"
#include "PlayerChar/ToroSideScrollerPlayer.h"

#define CLASSNAME AToroSideScrollerPlayer
class TOROEDITOR_API FSideScrollerPlayerCustomization final : public FToroActorCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FSideScrollerPlayerCustomization>();
	}

private:
	
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
	}
};
#undef CLASSNAME