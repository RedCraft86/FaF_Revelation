// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroCharacter.h"
#include "DataTypes/LightingData.h"
#include "Helpers/GameplayTagMacros.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroPlayerCharacter.generated.h"

class AToroGameMode;
class AToroGameState;
class AToroPlayerController;

namespace PlayerLockTags
{
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(PlayerLock)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Loading)

	TORORUNTIME_API CREATE_TAG_VERIFIER(PlayerLock)
}

UCLASS(Abstract)
class TORORUNTIME_API AToroPlayerCharacter : public AToroCharacter
{
	GENERATED_BODY()

public:

	AToroPlayerCharacter();

	PLAYER_CLASS_GETTER(AToroPlayerCharacter, GetPlayerPawn);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UPointLightComponent> PlayerLight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UCameraComponent> PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UAudioComponent> FootstepAudio;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class USceneComponent> EquipmentRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UInteractionManager> Interaction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tick, meta = (ClampMin = 0.05f, UIMin = 0.05f))
		float SlowTickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (Categories = "PlayerLock"))
		FGameplayTagContainer LockTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPointLightProperties LightSettings;

	UFUNCTION(BlueprintPure, Category = Player)
		float GetSpeed() const { return GetVelocity().Size2D(); }
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsMoving() const { return GetSpeed() > 50.0f; }
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsPaused() const { return GetWorldSettings()->GetPauserPlayerState() != nullptr; }

	UFUNCTION(BlueprintPure, Category = Player)
		virtual bool IsLocked();

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void AddLockTag(UPARAM(meta = (Categories = "PlayerLock")) const FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void RemoveLockTag(UPARAM(meta = (Categories = "PlayerLock")) const FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void ClearLockTags();

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void SetLockOnTarget(AActor* InTarget);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void ClearLockOnTarget();

	UFUNCTION(BlueprintPure, Category = Player)
		AActor* GetLockOnTarget() const { return LockOnTarget.Get(); }

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void SetLightSettings(const FPointLightProperties& InSettings);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void PlayFootstep(USoundBase* InSound);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Pawn)
		void SetControlRotation(const FRotator& InRotator) const;

	template <typename T = AToroPlayerController>
	T* GetPlayerController() const { return GetController<T>(); }

	template <typename T = AToroGameMode>
	T* GetGameMode() const { return GetWorld()->GetAuthGameMode<T>(); }

	template <typename T = AToroGameState>
	T* GetGameState() const { return GetWorld()->GetGameState<T>(); }

	virtual bool GetViewTarget_Implementation(FVector& Location) const override;
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const override;

protected:

	float SlowTickTime;
	TWeakObjectPtr<AActor> LockOnTarget;

	float GetCapsuleVerticalOffset(const float CapLerp = 0.4f) const;

	virtual void SlowTick() {}
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
