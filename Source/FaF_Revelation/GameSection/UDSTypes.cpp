// Copyright (C) RedCraft86. All Rights Reserved.

#include "UDSTypes.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_Weather, "Weather")
UE_DEFINE_GAMEPLAY_TAG(Tag_FogThin, "Weather.FogThin");
UE_DEFINE_GAMEPLAY_TAG(Tag_FogThick, "Weather.FogThick");
UE_DEFINE_GAMEPLAY_TAG(Tag_Cloudy, "Weather.Cloudy");
UE_DEFINE_GAMEPLAY_TAG(Tag_Overcast, "Weather.Overcast");
UE_DEFINE_GAMEPLAY_TAG(Tag_PartlyCloudy, "Weather.PartlyCloudy");
UE_DEFINE_GAMEPLAY_TAG(Tag_Thunderstorm, "Weather.Thunderstorm");

UWorld* UUDSSetterObject::GetWorld() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return GWorld;
#endif
	UWorld* World = Super::GetWorld();
	if (!World) World = GEngine ? GEngine->GetCurrentPlayWorld() : GWorld;
	return World;
}
