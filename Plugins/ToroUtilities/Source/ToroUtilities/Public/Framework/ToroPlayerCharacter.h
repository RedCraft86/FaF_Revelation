// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroPlayerController.h"
#include "Actors/ToroCharacter.h"
#include "Helpers/ClassGetters.h"
#include "DataTypes/LightingData.h"
#include "DataTypes/PlayerLockFlag.h"
#include "ToroPlayerCharacter.generated.h"

class AToroGameMode;
class UToroGameInstance;

#define LockFlag(Flag) PlayerLockTags::TAG_##Flag.GetTag()
#define AddPlayerLock(Flag) AddLockFlag(LockFlag(Flag))
#define ClearPlayerLock(Flag) ClearLockFlag(LockFlag(Flag))

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
		TArray<FPlayerLockFlag> LockFlags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPointLightProperties LightSettings;

	UFUNCTION(BlueprintPure, Category = Player)
		AToroGameMode* GetGameMode() const { return GameMode; }

	UFUNCTION(BlueprintPure, Category = Player)
		AToroPlayerController* GetPlayerController() const { return GetController<AToroPlayerController>(); }
	
	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void AddLockFlag(const FPlayerLockFlag& InFlag);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void ClearLockFlag(const FPlayerLockFlag& InFlag);

	UFUNCTION(BlueprintPure, Category = Player)
		virtual bool HasLockFlag(const FPlayerLockFlag& InFlag) const;

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
