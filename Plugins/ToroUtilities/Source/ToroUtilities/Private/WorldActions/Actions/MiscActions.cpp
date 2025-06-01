// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/MiscActions.h"
#include "MiscClasses/ToroSequenceActor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/AmbientSound.h"
#include "Helpers/LatentInfo.h"

void FWALevelStreaming::RunAction()
{
	if (Level.IsNull()) return;
	if (bUnloadLevel)
	{
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorldContext(),
			Level, LatentInfo::Make(), false);
	}
	else
	{
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorldContext(), Level,
			bMakeVisibleAfterLoad, false, LatentInfo::Make());
	}
}

void FWALevelSequence::RunAction()
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

void FWAWorldSound::RunAction()
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

void FWASound2D::RunAction()
{
	UGameplayStatics::PlaySound2D(GetWorldContext(), Sound, Volume, Pitch);
}
