﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCore.h"
#include "ExecPinEnums.h"
#include "LightingData.h"
#include "ToroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DataTypes/PlayerLockFlag.h"
#include "Components/PointLightComponent.h"
#include "ToroPlayerCharacter.generated.h"

class AToroGameMode;
class AToroPlayerController;

UE_DECLARE_GAMEPLAY_TAG_CHILD(Player, Character)

UCLASS()
class TOROUTILITIES_API AToroPlayerCharacter : public AToroCharacter
{
	GENERATED_BODY()

public:

	AToroPlayerCharacter();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UPointLightComponent> PlayerLight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> EquipmentRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tick, meta = (ClampMin = 0.05f, UIMin = 0.05f))
		float SlowTickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharSettings)
		TArray<FPlayerLockFlag> LockFlags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPointLightProperties LightSettings;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "ContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Player Character"))
	static EToroValidPins GetToroPlayerCharacter(AToroPlayerCharacter*& OutObject, const UObject* ContextObject, const TSubclassOf<AToroPlayerCharacter>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroPlayerCharacter>
	static T* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerCharacter(ContextObject, PlayerIndex));
	}
	
	UFUNCTION(BlueprintPure, Category = Player)
		AToroGameMode* GetGameMode() const { return GameMode; }

	UFUNCTION(BlueprintPure, Category = Player)
		AToroPlayerController* GetPlayerController() const { return PlayerController; }

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

	UFUNCTION(BlueprintNativeEvent)
		class UInventoryComponent* GetInventory() const;
	virtual UInventoryComponent* GetInventory_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintNativeEvent)
		class UToroNarrativeComponent* GetNarrative() const;
	virtual UToroNarrativeComponent* GetNarrative_Implementation() const { return nullptr; }

protected:

	UPROPERTY() FTimerHandle SlowTickTimer;
	UPROPERTY(Transient) TObjectPtr<AToroGameMode> GameMode;
	UPROPERTY(Transient) TObjectPtr<AToroPlayerController> PlayerController;
	UPROPERTY(Transient) TObjectPtr<class UToroGameInstance> GameInstance;

	virtual void SlowTick() {}
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
