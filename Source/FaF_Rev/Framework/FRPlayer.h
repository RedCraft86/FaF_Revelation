﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PlayerData.h"
#include "GameFramework/Character.h"
#include "Inspection/InspectionActor.h"
#include "PhotoMode/PhotoModeActor.h"
#include "FRPlayer.generated.h"

#define FRPlayer(Context) AFRPlayerBase::Get<AFRPlayerBase>(Context)

class UCameraComponent;
class USpringArmComponent;
class USpotLightComponent;

UCLASS(Abstract)
class FAF_REV_API AFRPlayerBase final : public ACharacter
{
	GENERATED_BODY()

public:

	AFRPlayerBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "DefaultSubobjects")
		USpringArmComponent* CameraArm;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "DefaultSubobjects")
		UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "DefaultSubobjects")
		USceneComponent* EquipmentRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "DefaultSubobjects")
		UAudioComponent* FootstepAudio;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "DefaultSubobjects")
		USpotLightComponent* PlayerLight;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (Bitmask, BitmaskEnum = "/Script/FaF_Rev.EPlayerControlFlags"))
		int32 ControlFlags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", AdvancedDisplay)
		TSoftObjectPtr<APhotoModeActor> PhotoModeActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", AdvancedDisplay)
		TSoftObjectPtr<AInspectionActor> InspectionActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", AdvancedDisplay, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<FName, class UInputAction*> InputActions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", AdvancedDisplay)
		FPlayerCameraShakes CameraShakes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", AdvancedDisplay)
		FPlayerFootsteps FootstepSounds;

	virtual void SetActorHiddenInGame(bool bNewHidden) override;

protected:

	FPlayerInteraction InteractData;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public: // Statics
	
	UFUNCTION(BlueprintPure, Category = "Game", DisplayName = "Get Player (Smart)", meta = (DynamicOutputParam = "ReturnValue", DeterminesOutputType = "Class", WorldContext = "WorldContextObject"))
		static AFRPlayerBase* GetPlayerPawnSmart(const UObject* WorldContextObject, const TSubclassOf<AFRPlayerBase> Class);
	template <typename T = AFRPlayerBase>
	static T* Get(const UObject* WorldContextObject)
	{
		return Cast<T>(GetPlayerPawnSmart(WorldContextObject, T::StaticClass()));
	}
};
