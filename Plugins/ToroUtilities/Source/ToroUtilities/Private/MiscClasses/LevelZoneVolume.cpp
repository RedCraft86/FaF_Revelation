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
	//TODO
}

void ALevelZoneVolume::BeginPlay()
{
	Super::BeginPlay();
	GameState = AToroGameState::Get(this);
	CamManager = AToroCameraManager::Get(this);
	MusicManager = AToroMusicManager::Get(this);

	FTimerManager& Manager = GetWorldTimerManager();
	Manager.SetTimer(CullingTimer, this,
		&ALevelZoneVolume::UpdateSmartCulling, 0.1f, true);
	if (!IsEnabled()) Manager.PauseTimer(CullingTimer);
}

void ALevelZoneVolume::OnEnableStateChanged(const bool bIsEnabled)
{
	Super::OnEnableStateChanged(bIsEnabled);
	if (FTimerManager& Manager = GetWorldTimerManager(); Manager.TimerExists(CullingTimer))
	{
		bIsEnabled ? Manager.UnPauseTimer(CullingTimer) : Manager.PauseTimer(CullingTimer);
	}
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
