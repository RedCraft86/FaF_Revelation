// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldEvents/WorldEventActor.h"

AWorldEventActor::AWorldEventActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WorldEvents = CreateDefaultSubobject<UWorldEventComponent>("WorldEvents");

#if WITH_EDITORONLY_DATA
	BillboardIcon = TEXT("/Engine/EditorResources/S_Trigger.S_Trigger");
#endif
}

void AWorldEventActor::RunEvents() const
{
	WorldEvents->RunEvents();
}
