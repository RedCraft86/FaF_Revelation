// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/SoundActions.h"
#include "Kismet/GameplayStatics.h"

void FWTaskWorldSound::RunEvent(const UObject* WorldContext)
{
	FOR_EACH_SOFT_PTR(Targets, AFMODAmbientSound, {
		if (UFMODAudioComponent* AudioComp = Ptr->AudioComponent)
		{
			bStopSound ? AudioComp->Stop() : AudioComp->Play();
		}
	})
}

void FWTaskSound2D::RunEvent(const UObject* WorldContext)
{
	UGameplayStatics::PlaySound2D(WorldContext, Sound, Volume, Pitch);
}
