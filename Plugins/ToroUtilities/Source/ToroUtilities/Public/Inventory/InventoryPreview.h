﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MathTypes.h"
#include "InputAction.h"
#include "GameFramework/Actor.h"
#include "InventoryPreview.generated.h"

UCLASS(Abstract)
class TOROUTILITIES_API AInventoryPreview final : public AActor
{
	GENERATED_BODY()

	friend class UInventoryComponent;

public:

	AInventoryPreview();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class USpringArmComponent> PreviewRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UStaticMeshComponent> PreviewMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class USpotLightComponent> PreviewLight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneCaptureComponent2D> PreviewCapture;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D TurnSpeedRate;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D ZoomSpeedRate;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayThumbnail = false))
		TObjectPtr<UInputAction> TurnInput;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayThumbnail = false))
		TObjectPtr<UInputAction> ZoomInput;

	bool SetItem(const FGuid& InSlotID);

protected:

	FGuid SlotID;
	FVector2D ZoomRange;
	FRotator RotationValue;
	FToroFloatInterp ZoomValue;
	UPROPERTY(Transient) TObjectPtr<UInventoryComponent> Inventory;

	void Initialize();
	void Deinitialize();
	void InputBinding_Turn(const FInputActionValue& InValue);
	void InputBinding_Zoom(const FInputActionValue& InValue);
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
