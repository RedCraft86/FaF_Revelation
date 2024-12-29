// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldEvents/Events/SoundEvents.h"
#include "Kismet/GameplayStatics.h"

void FWEAmbientSound::RunEvent(const UObject* WorldContext)
{
	for (const TSoftObjectPtr<AAmbientSound>& Actor : Targets)
	{
		const AAmbientSound* ActorPtr = Actor.LoadSynchronous();
		if (UAudioComponent* AudioComp = ActorPtr ? ActorPtr->GetAudioComponent() : nullptr)
		{
			if (bFade && FadeTime > 0.0f)
			{
				bStopSound ? AudioComp->FadeOut(FadeTime, 0.0f, FadeCurve) :
					AudioComp->FadeIn(FadeTime, 1.0f, StartTime, FadeCurve);
			}
			else
			{
				bStopSound ? AudioComp->Stop() : AudioComp->Play();
			}
		}
	}
}

void FWESound2D::RunEvent(const UObject* WorldContext)
{
	UGameplayStatics::PlaySound2D(WorldContext, Sound, Volume, Pitch);	
}
