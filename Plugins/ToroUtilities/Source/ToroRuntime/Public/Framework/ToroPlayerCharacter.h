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
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(MainMenu)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Loading)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Dialogue)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Cinematic)

	TORORUNTIME_API CREATE_TAG_VERIFIER(PlayerLock)
}

UCLASS()
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
		TObjectPtr<class UInspectionManager> Inspection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UInteractionManager> Interaction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tick, meta = (ClampMin = 0.05f, UIMin = 0.05f))
		float SlowTickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FGameplayTagContainer LockTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPointLightProperties LightSettings;

	UFUNCTION(BlueprintCallable, Category = Player)
		void AddLockTag(UPARAM(meta = (Categories = "PlayerLock")) const FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = Player)
		void ClearLockTag(UPARAM(meta = (Categories = "PlayerLock")) const FGameplayTag InTag);

	UFUNCTION(BlueprintPure, Category = Player)
		bool HasLockTag(UPARAM(meta = (Categories = "PlayerLock")) const FGameplayTag InTag) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		virtual void ExitCinematic();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		virtual void EnterCinematic(AActor* InInstigator);

	template <typename T = AToroPlayerController>
	T* GetPlayerController() const { return GetController<T>(); }

	template <typename T = AToroGameMode>
	T* GetGameMode() const { return GetWorld()->GetAuthGameMode<T>(); }

	template <typename T = AToroGameState>
	T* GetGameState() const { return GetWorld()->GetGameState<T>(); }

protected:

	float SlowTickTime;

	virtual void SlowTick() {}
	virtual void OnLockTagsChanged() {}

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
