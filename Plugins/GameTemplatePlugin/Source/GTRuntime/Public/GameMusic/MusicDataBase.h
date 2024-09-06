// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "MusicDataBase.generated.h"

UENUM(BlueprintType)
enum class EMusicChannel : uint8
{
	// Default channel
	Main,

	// AI not sensed player
	Regular,

	// AI sensed player
	Sensed,

	// AI chase player
	Chase,

	// AI search player
	Search
};
ENUM_RANGE_BY_FIRST_AND_LAST(EMusicChannel, EMusicChannel::Regular, EMusicChannel::Search);

USTRUCT(BlueprintType)
struct GTRUNTIME_API FMusicChannelData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "MusicChannelData")
		TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = "MusicChannelData", meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Volume;

	FMusicChannelData() : Sound(nullptr), Volume(1.0f) {}
};

UCLASS()
class GTRUNTIME_API UMusicDataBase : public UDataAsset
{
	GENERATED_BODY()

public:

	UMusicDataBase() {}
	
	static inline TSubclassOf<UMusicDataBase> DefaultSpawnClass = StaticClass();

	UPROPERTY(EditAnywhere, Category = "WorldMusicData", meta = (TitleProperty = "Sound", ReadOnlyKeys))
		TMap<EMusicChannel, FMusicChannelData> Sounds;

#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		for (const EMusicChannel Channel : TEnumRange<EMusicChannel>())
		{
			Sounds.Remove(EMusicChannel::Main);
			if (!Sounds.Contains(Channel)) Sounds.Add(Channel);
		}
	}
#endif
};
