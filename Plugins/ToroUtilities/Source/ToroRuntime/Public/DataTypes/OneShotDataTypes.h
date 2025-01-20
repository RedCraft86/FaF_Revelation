﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ECFHandle.h"
#include "GameplayTagContainer.h"
#include "OneShotDataTypes.generated.h"

class AToroMusicManager;

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FOneShotEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = OneShot)
		TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(VisibleAnywhere, Category = OneShot)
		bool bIsLooping;
	
	UPROPERTY(EditAnywhere, Category = OneShot, meta = (ClampMin = 0.1f))
		float Volume;

	UPROPERTY(EditAnywhere, Category = OneShot, meta = (ClampMin = 0.1f))
		FVector FadeTimes;

	UPROPERTY(EditAnywhere, Category = OneShot, meta = (ClampMin = 0.0f))
		FVector2D StartRange;

	FOneShotEntry(): bIsLooping(false), Volume(1.0f), FadeTimes(1.0f), StartRange(0.0f) {}
#if WITH_EDITOR
	void Update();
#endif

	bool IsValidData() const { return IsValid(Sound.LoadSynchronous()); }
	float GetStartTime() const { return FMath::RandRange(StartRange.GetMin(), StartRange.GetMax()); }
};

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UOneShotDatabase final : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UOneShotDatabase() {}

	UPROPERTY(EditAnywhere, Category = OneShot, meta = (ForceInlineRow, Categories = "OneShot"))
		TMap<FGameplayTag, FOneShotEntry> OneShots;

	static bool IsValidKey(const FGameplayTag& Key);
	static FOneShotEntry Get(const FGameplayTag& Key);
	
#if WITH_EDITOR
private:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FOneShotLayer
{
	GENERATED_BODY()

private:
	
	FECFHandle FadeHandle;
	FECFHandle PauseHandle;
	UPROPERTY() bool bPaused;
	UPROPERTY() bool bStopping;
	UPROPERTY() bool bAutoDestroy;
	UPROPERTY() FGameplayTag SoundID;
	UPROPERTY(Transient) TObjectPtr<UAudioComponent> Component;
	UPROPERTY(Transient) TSet<TObjectPtr<const UObject>> Instigators;
	UPROPERTY(Transient) TObjectPtr<AToroMusicManager> Owner;

public:
	
	void Stop();
	void Restart();
	void SetPaused(const bool bInPaused);
	void AddInstigator(const UObject* InObject);
	void RemoveInstigator(const UObject* InObject);
	
	bool IsLooping() const;
	bool IsValidLayer() const;
	bool CanRunFunctions() const;
	void Initialize(AToroMusicManager* InOwner, const UObject* Instigator);
	void OnAudioFinished(UAudioComponent* Comp);

	FOneShotLayer() : bPaused(false), bStopping(false), bAutoDestroy(false) {}
	FOneShotLayer(const FGameplayTag& InSoundID)
		: bPaused(false), bStopping(false), bAutoDestroy(false), SoundID(InSoundID)
	{}
};