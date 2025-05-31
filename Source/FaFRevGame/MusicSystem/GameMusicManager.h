// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "Framework/ToroGameState.h"
#include "GameMusicManager.generated.h"

UCLASS()
class FAFREVGAME_API AGameMusicManager final : public AToroGameState
{
	GENERATED_BODY()

public:

	AGameMusicManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> ThemeComponent;

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
		void SetThemeHiding(const bool bInHiding) const;

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void SetDipped(const bool bInDipped) const;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateChanged, const uint8);
	FOnStateChanged OnStateChanged;

private:

	UPROPERTY() FTimerHandle ChangeTimer;
	IAudioParameterControllerInterface* GetSoundParamInterface() const;

	virtual void BeginPlay() override;
};
