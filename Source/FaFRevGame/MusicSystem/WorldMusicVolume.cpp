// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldMusicVolume.h"
#include "Components/AudioComponent.h"

AWorldMusicVolume::AWorldMusicVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	MusicComponent = CreateDefaultSubobject<UAudioComponent>("MusicComponent");
	MusicComponent->SetupAttachment(GetRootComponent());
	MusicComponent->SetUISound(true);
	MusicComponent->bCanPlayMultipleInstances = false;
	MusicComponent->bAutoActivate = false;
	MusicComponent->bIsMusic = true;

#if WITH_EDITOR
	BrushColor = FColor(180, 0, 255);
#endif
}

#if WITH_EDITOR
void AWorldMusicVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
#endif
