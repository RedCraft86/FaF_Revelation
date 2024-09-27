// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MusicDataBase.h"
#include "GameFramework/Actor.h"
#include "MusicManager.generated.h"

USTRUCT(BlueprintType)
struct GTRUNTIME_API FOneShotAudioData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OneShotAudioData")
		FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OneShotAudioData")
		EMusicChannel Channel = EMusicChannel::Main;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OneShotAudioData")
		TObjectPtr<UAudioComponent> Component;
};

UCLASS(NotBlueprintable)
class GTRUNTIME_API AMusicManager : public AActor
{
	GENERATED_BODY()

public:

	AMusicManager();
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TMap<EMusicChannel, TObjectPtr<UAudioComponent>> AudioComponents;

	UFUNCTION(BlueprintPure, Category = "MusicManager")
		bool IsChannelMuted(const EMusicChannel InChannel) const { return bChannelStates.FindRef(InChannel); }
	
	UFUNCTION(BlueprintCallable, Category = "MusicManager")
		void SetChannelMuted(const EMusicChannel InChannel, const bool bMuted, const bool bImmediate = false);

	UFUNCTION(BlueprintCallable, Category = "MusicManager")
		void MuteAllChannels(const bool bIncludeMainChannel = false, const bool bImmediate = false);
	
	UFUNCTION(BlueprintCallable, Category = "MusicManager")
		void PlayOneShotAudio(USoundBase* InSound, const EMusicChannel InChannel, const float InVolume = 1.0f, const FString InOptionalID = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "MusicManager")
		void SetBaseMusicData(UMusicDataBase* InData);

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnChannelStateChanged, const EMusicChannel, const bool /*bMuted*/, const bool /*bImmediate*/);
	FOnChannelStateChanged OnChannelStateChanged;

protected:

	UPROPERTY(Transient)
		TObjectPtr<UMusicDataBase> BaseMusicData;
	
	UPROPERTY(Transient)
		TMap<EMusicChannel, bool> bChannelStates;

	UPROPERTY(Transient)
		TArray<FOneShotAudioData> OneShotComponents;

	void ClearStoppedOneShots();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public: // Statics
	
	UFUNCTION(BlueprintPure, Category = "Game", DisplayName = "Get Music Manager", meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "Class"))
		static AMusicManager* GetMusicManager(const UObject* WorldContextObject);

	template <typename T = AMusicManager>
	static T* Get(const UObject* WorldContextObject)
	{
		return Cast<T>(GetMusicManager(WorldContextObject));
	}
};
