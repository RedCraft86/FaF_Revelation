// Copyright (C) RedCraft86. All Rights Reserved.

#include "MiscClasses/LevelZoneVolume.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroCameraManager.h"
#include "Framework/ToroMusicManager.h"
#include "Framework/ToroGameState.h"

ALevelZoneVolume::ALevelZoneVolume()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
#if WITH_EDITORONLY_DATA
	BrushColor = FColor::MakeRandomColor();
#endif
}

void ALevelZoneVolume::UpdateSmartCulling()
{
}

void ALevelZoneVolume::BeginPlay()
{
	Super::BeginPlay();
	GameState = AToroGameState::Get(this);
	CamManager = AToroCameraManager::Get(this);
	MusicManager = AToroMusicManager::Get(this);

	GetWorldTimerManager().SetTimer(CullingTimer, this,
		&ALevelZoneVolume::UpdateSmartCulling, 0.1f, true);
}

void ALevelZoneVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!IsEnabled() || !GameState) return;
	if (const AToroCharacter* Char = Cast<AToroCharacter>(OtherActor))
	{
		GameState->AssignCharacterToZone(Char->GetCharacterID(), ZoneID);
	}
	if (const AToroPlayerCharacter* Player = Cast<AToroPlayerCharacter>(OtherActor))
	{
		// TODO
	}
}

void ALevelZoneVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (!IsEnabled() || !GameState) return;
	if (const AToroPlayerCharacter* Player = Cast<AToroPlayerCharacter>(OtherActor))
	{
		// TODO
	}
}
