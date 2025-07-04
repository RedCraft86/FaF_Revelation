﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "MiscObjects/UDSSetter.h"

namespace WeatherTags
{
	DEFINE_GAMEPLAY_TAG(Weather)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, ClearSkies)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, Cloudy)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, Overcast)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, PartlyCloudy)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, FogThick)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, FogThin)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, Rain)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, RainLight)
	DEFINE_GAMEPLAY_TAG_CHILD(Weather, Thunderstorm)
}