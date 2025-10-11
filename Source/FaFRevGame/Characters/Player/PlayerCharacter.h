// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PlayerTypes.h"
#include "Framework/ToroPlayerCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API APlayerCharacter final : public AToroPlayerCharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

protected:

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerControlFlags"))
		int32 ControlFlags;

	UPROPERTY(VisibleAnywhere, Category = Settings, meta = (Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerStateFlags"))
		int32 StateFlags;

public:

	UFUNCTION(BlueprintCallable, Category = Player)
		void OverrideControlFlags(const int32 InFlags);

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetControlFlag(const EPlayerControlFlags InFlag);

	UFUNCTION(BlueprintCallable, Category = Player)
		void UnsetControlFlag(const EPlayerControlFlags InFlag);

	UFUNCTION(BlueprintPure, Category = Player)
		bool HasControlFlag(const EPlayerControlFlags InFlag) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetStateFlag(const EPlayerStateFlags InFlag);

	UFUNCTION(BlueprintCallable, Category = Player)
		void UnsetStateFlag(const EPlayerStateFlags InFlag);

	UFUNCTION(BlueprintPure, Category = Player)
		bool HasStateFlag(const EPlayerStateFlags InFlag) const;

	UFUNCTION(BlueprintPure, Category = Player)
		int32 GetActivityFlags() const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetHidingSpot(AActor* InSpot);

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetOperatingActor(AActor* InActor);

protected:

	TWeakObjectPtr<AActor*> HidingSpot;
	TWeakObjectPtr<AActor*> OperatingActor;
	TObjectPtr<class UTutorialManager> TutorialManager;
	TObjectPtr<class UInventoryManager> InventoryManager;
	TObjectPtr<class UInspectionManager> InspectionManager;
	TObjectPtr<class UNarrativeManager> NarrativeManager;

	virtual bool ShouldLockPlayer() override;

	virtual void SlowTick() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
