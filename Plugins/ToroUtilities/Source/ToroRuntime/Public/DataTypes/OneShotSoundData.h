// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ECFHandle.h"
#include "OneShotSoundData.generated.h"

class AToroMusicManager;

USTRUCT()
struct TORORUNTIME_API FOneShotSoundData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = MusicData)
		TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(VisibleAnywhere, Category = MusicData)
		bool bIsLooping;
	
	UPROPERTY(EditAnywhere, Category = MusicData, meta = (ClampMin = 0.1f))
		float Volume;

	UPROPERTY(EditAnywhere, Category = MusicData, meta = (ClampMin = 0.1f))
		FVector FadeTimes;

	UPROPERTY(EditAnywhere, Category = MusicData, meta = (ClampMin = 0.0f))
		FVector2D StartRange;

	FOneShotSoundData(): bIsLooping(false), Volume(1.0f), FadeTimes(1.0f), StartRange(0.0f) {}
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;

	bool IsValidData() const { return IsValid(Sound.LoadSynchronous()); }
	float GetStartTime() const { return FMath::RandRange(StartRange.GetMin(), StartRange.GetMax()); }

	static bool IsValidKey(const FName& Key);
	static FOneShotSoundData Get(const FName& Key);
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FOneShotSoundLayer
{
	GENERATED_BODY()

private:
	
	FECFHandle FadeHandle;
	FECFHandle PauseHandle;
	UPROPERTY() FName SoundID;
	UPROPERTY() bool bPaused;
	UPROPERTY() bool bStopping;
	UPROPERTY() bool bAutoDestroy;
	UPROPERTY(Transient) TObjectPtr<UAudioComponent> Component;
	UPROPERTY(Transient) TSet<TObjectPtr<const UObject>> Instigators;
	UPROPERTY(Transient) TObjectPtr<AToroMusicManager> Owner;

public:
	
	void Stop();
	void Restart();
	void SetPaused(const bool bInPaused);
	void AddInstigator(const UObject* InObject);
	void RemoveInstigator(const UObject* InObject);
	
	bool IsValidLayer() const;
	bool CanRunFunctions() const;
	void Initialize(AToroMusicManager* InOwner, const UObject* Instigator);
	void OnAudioFinished(UAudioComponent* Comp);

	FOneShotSoundLayer() : bPaused(false), bStopping(false), bAutoDestroy(false) {}
	FOneShotSoundLayer(const FName& InSoundID)
		: SoundID(InSoundID), bPaused(false), bStopping(false), bAutoDestroy(false)
	{}
};