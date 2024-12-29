// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldEvents/Events/MiscEvents.h"
#include "Engine/LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

void FWEEventRemote::RunEvent(const UObject* WorldContext)
{
	if (EventName.IsNone()) return;
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (ALevelScriptActor* LSA = World ? World->GetLevelScriptActor() : nullptr)
	{
		LSA->RemoteEvent(EventName);
	}
}

void FWEEventActor::RunEvent(const UObject* WorldContext)
{
	if (EventName.IsNone()) return;
	for (const TSoftObjectPtr<AActor>& Actor : Targets)
	{
		if (AActor* ActorPtr = Actor.LoadSynchronous())
		{
			FOutputDeviceNull Ar;
			ActorPtr->CallFunctionByNameWithArguments(*EventName.ToString(),
				Ar, nullptr, true);
		}
	}
}

void FWESequencer::RunEvent(const UObject* WorldContext)
{
	for (TSoftObjectPtr<ALevelSequenceActor> Actor : Targets)
	{
		const ALevelSequenceActor* ActorPtr = Actor.LoadSynchronous();
		if (ULevelSequencePlayer* Player = ActorPtr ? ActorPtr->GetSequencePlayer() : nullptr)
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
	}
}

void FWELevelStreamLoad::RunEvent(const UObject* WorldContext)
{
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(WorldContext, World,
		bMakeVisibleAfterLoad, false, GetLatentInfo());
}

void FWELevelStreamUnload::RunEvent(const UObject* WorldContext)
{
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(WorldContext, World,
		GetLatentInfo(), false);
}
