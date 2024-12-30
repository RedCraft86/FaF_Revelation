// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/MiscActions.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

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
	FOR_EACH_SOFT_PTR(Targets, ALevelSequenceActor, {
		if (ULevelSequencePlayer* Player = Ptr->GetSequencePlayer())
		{
			if (bStopPlaying)
			{
				Player->GoToEndAndStop();
			}
			else if (!FMath::IsNearlyZero(PlayRate))
			{
				Player->SetPlayRate(FMath::Abs(PlayRate));
				if (PlayRate < 0.0f)
				{
					Player->GoToEndAndStop();
					Player->PlayReverse();
				}
				else
				{
					Player->StopAtCurrentTime();
					Player->SetPlaybackPosition({0.0f, EUpdatePositionMethod::Jump});
					Player->Play();
				}
			}
			else return;
		}
	})
}
