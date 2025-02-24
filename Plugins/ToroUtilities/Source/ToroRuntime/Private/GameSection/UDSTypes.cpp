// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameSection/UDSTypes.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_Weather, "Weather")

UWorld* UUDSSetterObject::GetWorld() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return GEngine->GetCurrentPlayWorld();
#endif
	return UObject::GetWorld();
}
