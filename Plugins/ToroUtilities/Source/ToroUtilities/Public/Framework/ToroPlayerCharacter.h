// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroPlayerController.h"
#include "Actors/ToroCharacter.h"
#include "Helpers/ClassGetters.h"
#include "DataTypes/LightingData.h"
#include "DataTypes/PlayerLockTag.h"
#include "ToroPlayerCharacter.generated.h"

class AToroGameMode;
class UToroGameInstance;

UCLASS()
class TOROUTILITIES_API AToroPlayerCharacter : public AToroCharacter
{
	GENERATED_BODY()

public:

	AToroPlayerCharacter();

	PLAYER_CLASS_GETTER(AToroPlayerCharacter, GetPlayerPawn);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UPointLightComponent> PlayerLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tick, meta = (ClampMin = 0.05f, UIMin = 0.05f))
		float SlowTickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FGameplayTagContainer LockTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPointLightProperties LightSettings;

	UFUNCTION(BlueprintPure, Category = Player)
		AToroGameMode* GetGameMode() const { return GameMode; }

	UFUNCTION(BlueprintPure, Category = Player)
		AToroPlayerController* GetPlayerController() const { return GetController<AToroPlayerController>(); }
	
	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void AddLockTag(UPARAM(meta = (Categories = "PlayerLock")) const FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void ClearLockTag(UPARAM(meta = (Categories = "PlayerLock")) const FGameplayTag InTag);

	UFUNCTION(BlueprintPure, Category = Player)
		virtual bool HasLockTag(UPARAM(meta = (Categories = "PlayerLock")) const FGameplayTag InTag) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void SetLightSettings(const FPointLightProperties& InSettings);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		virtual void EnterCinematic(AActor* CinematicActor);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		virtual void ExitCinematic();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		virtual void FadeToBlack(const float InTime, const bool bAudio = true) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		virtual void FadeFromBlack(const float InTime, const bool bAudio = true) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		virtual void ClearFade() const;
	
protected:

	UPROPERTY() FTimerHandle SlowTickTimer;
	UPROPERTY(Transient) TObjectPtr<AToroGameMode> GameMode;
	UPROPERTY(Transient) TObjectPtr<UToroGameInstance> GameInstance;

	virtual void SlowTick() {}
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
