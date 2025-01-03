// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/SoundActions.h"
#include "Kismet/GameplayStatics.h"

void FWTaskWorldSound::RunEvent(const UObject* WorldContext)
{
	FOR_EACH_SOFT_PTR(Targets, AAmbientSound, {
		if (UAudioComponent* AudioComp = Ptr->GetAudioComponent())
		{
			if (bFade)
			{
				bStopSound ? AudioComp->FadeOut(FadeTime, 0.0f, FadeCurve) :
					AudioComp->FadeIn(FadeTime, 1.0f, StartTime, FadeCurve);
			}
			else
			{
				bStopSound ? AudioComp->Stop() : AudioComp->Play();
			}
		}
	})
}

void FWTaskSound2D::RunEvent(const UObject* WorldContext)
{
	UGameplayStatics::PlaySound2D(WorldContext, Sound, Volume, Pitch);
}
