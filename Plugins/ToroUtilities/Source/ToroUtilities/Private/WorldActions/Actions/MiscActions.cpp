// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/MiscActions.h"
#include "MiscClasses/ToroSequenceActor.h"
#include "Kismet/GameplayStatics.h"

void FWTaskLevelStreaming::RunEvent(const UObject* WorldContext)
{
	if (Level.IsNull()) return;
	if (bUnloadLevel)
	{
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(WorldContext, Level,
			GetLatentInfo(), false);
	}
	else
	{
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(WorldContext, Level,
			bMakeVisibleAfterLoad, false, GetLatentInfo());
	}
}

void FWTaskLevelSequence::RunEvent(const UObject* WorldContext)
{
	FOR_EACH_SOFT_PTR(Targets, AToroSequenceActor, {
		if (bStopPlaying)
		{
			Ptr->SkipToEnd();
		}
		else if (!FMath::IsNearlyZero(PlayRate))
		{
			Ptr->SetPlayRate(FMath::Abs(PlayRate));
			if (PlayRate < 0.0f)
			{
				Ptr->SkipToEnd();
				Ptr->Reverse();
			}
			else
			{
				Ptr->StopAtCurrentTime();
				Ptr->SetPlaybackPosition({0.0f, EUpdatePositionMethod::Jump});
				Ptr->Play();
			}
		}
	})
}

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
