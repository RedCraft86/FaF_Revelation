// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Framework/ToroGameState.h"
#include "GameMusicManager.generated.h"

UCLASS()
class FAFREVGAME_API AGameMusicManager final : public AToroGameState
{
	GENERATED_BODY()

public:

	AGameMusicManager();

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = "ContextObject"))
	static AGameMusicManager* GetMusicManager(const UObject* ContextObject)
	{
		return Get<AGameMusicManager>(ContextObject);
	}

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void ChangeMainTheme(UMetaSoundSource* InTheme);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void SetThemeMuted(const bool bInMuted);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void SetThemeState(const uint8 InState) const;

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void SetThemeIntensity(const float InIntensity) const;

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void AddDipRequest(const UObject* InObject);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void RemoveDipRequest(const UObject* InObject);

	DECLARE_MULTICAST_DELEGATE(FOnAudioDipped);
	FOnAudioDipped OnAudioDipped;

	DECLARE_MULTICAST_DELEGATE(FOnThemeChanged);
	FOnThemeChanged OnThemeChanged;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateChanged, const uint8);
	FOnStateChanged OnStateChanged;

private:

	UPROPERTY(VisibleAnywhere, Category = Subobjects)
		TObjectPtr<UAudioComponent> ThemeComponent;

	UPROPERTY() FTimerHandle ChangeTimer;
	UPROPERTY(Transient) TSet<TWeakObjectPtr<const UObject>> DipRequests;

	void UpdateDipState();
	virtual void BeginPlay() override;
	IAudioParameterControllerInterface* GetSoundParamInterface() const
	{
		return Cast<IAudioParameterControllerInterface>(ThemeComponent);
	}
};
