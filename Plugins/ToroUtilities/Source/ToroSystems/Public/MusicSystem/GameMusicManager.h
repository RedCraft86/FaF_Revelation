// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "Components/AudioComponent.h"
#include "GameMusicManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TOROSYSTEMS_API UGameMusicManager : public UAudioComponent
{
	GENERATED_BODY()

public:

	UGameMusicManager();

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = "ContextObject"))
		static UGameMusicManager* GetMusicManager(const UObject* ContextObject, const int32 PlayerIndex = 0);

	template <typename T = UGameMusicManager>
	static T* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(GetMusicManager(ContextObject, PlayerIndex));
	}

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void ChangeMainTheme(UMetaSoundSource* InTheme);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void SetThemeState(const uint8 InState);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void SetThemeIntensity(const float InIntensity);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void AddDipRequest(const UObject* InObject);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void RemoveDipRequest(const UObject* InObject);

	DECLARE_MULTICAST_DELEGATE(FOnMusicUpdate);
	FOnMusicUpdate OnThemeChanged;
	FOnMusicUpdate OnAudioDipped;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateChanged, const uint8);
	FOnStateChanged OnStateChanged;

protected:

	UPROPERTY() uint8 StateIdx;
	UPROPERTY() bool bGamePaused;
	UPROPERTY() FTimerHandle ChangeTimer;
	UPROPERTY(Transient) TSet<TWeakObjectPtr<const UObject>> DipRequests;

	void UpdateDipState();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
	IAudioParameterControllerInterface* GetParamInterface();
};
