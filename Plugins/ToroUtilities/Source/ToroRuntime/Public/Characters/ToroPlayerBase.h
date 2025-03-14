// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "LightingData.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/ToroGameMode.h"
#include "DataTypes/GenericPlayerTypes.h"
#include "Components/AudioComponent.h"
#include "Framework/ToroGameInstance.h"
#include "Characters/ToroCharacterBase.h"
#include "Components/PointLightComponent.h"
#include "ToroPlayerBase.generated.h"

class AToroNPCBase;
class AToroPlayerController;

UCLASS(Abstract)
class TORORUNTIME_API AToroPlayerBase : public AToroCharacterBase
{
	GENERATED_BODY()

public:
	
	AToroPlayerBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UPointLightComponent> PlayerLight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> FootstepAudio;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> EquipmentRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tick, meta = (ClampMin = 0.05f, UIMin = 0.05f))
		float SlowTickInterval;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterSettings, meta = (DisplayPriority = -1, Bitmask, BitmaskEnum = "/Script/ToroRuntime.EPlayerControlFlags"))
		int32 ControlFlags;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterSettings, meta = (DisplayPriority = -1, Bitmask, BitmaskEnum = "/Script/ToroRuntime.EPlayerStateFlags"))
		int32 StateFlags;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterSettings, meta = (DisplayPriority = -1))
		TSet<FName> LockFlags;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPointLightProperties LightSettings;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Player Character"))
		static EToroValidPins GetToroPlayerCharacter(AToroPlayerBase*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroPlayerBase>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroPlayerBase>
	static T* Get(const UObject* WorldContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex));
	}

	UFUNCTION(BlueprintPure, Category = Player)
		AToroGameMode* GetGameMode() const { return GameMode; }
	
	UFUNCTION(BlueprintPure, Category = Player)
		AToroPlayerController* GetPlayerController() const { return PlayerController; }

	UFUNCTION(BlueprintPure, Category = Player)
		AActor* GetCinematicActor() const { return CinematicActor; }

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void TeleportPlayer(const FVector& InLocation, const FRotator& InRotation);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void OverrideControlFlags(UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/ToroRuntime.EPlayerControlFlags")) const int32 InFlags);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void SetControlFlag(const EPlayerControlFlags InFlag);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void UnsetControlFlag(const EPlayerControlFlags InFlag);

	UFUNCTION(BlueprintPure, Category = Player)
		virtual bool HasControlFlag(const EPlayerControlFlags InFlag) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void SetStateFlag(const EPlayerStateFlags InFlag);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void UnsetStateFlag(const EPlayerStateFlags InFlag);

	UFUNCTION(BlueprintPure, Category = Player)
		virtual bool HasStateFlag(const EPlayerStateFlags InFlag) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void AddLockFlag(const FPlayerLockFlag& InFlag);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void ClearLockFlag(const FPlayerLockFlag& InFlag);

	UFUNCTION(BlueprintPure, Category = Player)
		virtual bool HasLockFlag(const FPlayerLockFlag& InFlag) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void ExitCinematic();

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void EnterCinematic(AActor* InActor);
	
	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void ClearEnemyStack();
	
	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void AddEnemy(AToroNPCBase* InEnemy);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void RemoveEnemy(AToroNPCBase* InEnemy);
	
	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void UpdateEnemy(const AToroNPCBase* InEnemy);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void SetLightSettings(const FPointLightProperties& InSettings);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		virtual void FadeToBlack(const float InTime, const bool bAudio = true) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		virtual void FadeFromBlack(const float InTime, const bool bAudio = true) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		virtual void ClearFade() const;
	
	virtual void EnterDialogue() {}

protected:

	UPROPERTY() FTimerHandle SlowTickTimer;
	UPROPERTY(Transient) TObjectPtr<AActor> CinematicActor;
	UPROPERTY(Transient) TObjectPtr<AToroGameMode> GameMode;
	UPROPERTY(Transient) TObjectPtr<AToroPlayerController> PlayerController;
	UPROPERTY(Transient) TObjectPtr<UToroGameInstance> GameInstance;
	UPROPERTY(Transient) TSet<TObjectPtr<AToroNPCBase>> EnemyStack;

	virtual void SlowTick() {}
	virtual void OnEnemyStackChanged() {}

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public: // Statics
	
	UFUNCTION(BlueprintPure, Category = PlayerData)
		static FName GetLockValue(const FPlayerLockFlag& InFlag) { return *InFlag; }
};
