// Copyright (C) RedCraft86. All Rights Reserved.

#include "LevelZones/LevelZoneVolume.h"
#include "Characters/ToroCharacterBase.h"
#include "Characters/ToroPlayerBase.h"
#include "Framework/ToroGameMode.h"

ALevelZoneVolume::ALevelZoneVolume()
{
}

void ALevelZoneVolume::BeginPlay()
{
	Super::BeginPlay();
	GameMode = AToroGameMode::Get(this);
}

void ALevelZoneVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (!IsEnabled() || !GameMode) return;
	if (const AToroCharacterBase* Char = Cast<AToroCharacterBase>(OtherActor))
	{
		GameMode->CharRoomMappings.Add(Char->GetCharacterID(), RoomID);
	}

	if (const AToroPlayerBase* Player = Cast<AToroPlayerBase>(OtherActor))
	{
	}
}

void ALevelZoneVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	if (!IsEnabled() && GameMode) return;
	if (const AToroPlayerBase* Player = Cast<AToroPlayerBase>(OtherActor))
	{
	}
}
