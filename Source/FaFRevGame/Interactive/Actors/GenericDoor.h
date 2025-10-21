// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

// Fixes MovieSceneNumericVariant.h(312,32): Error C4003 : not enough arguments for function-like macro invocation 'max'
// for whatever reason. It's actual magic.
#ifndef NOMINMAX
	#define NOMINMAX
#endif

#include "DataTypes/MathTypes.h"
#include "DataTypes/PrimitiveData.h"
#include "DataTypes/ToroInterpolator.h"
#include "Interaction/InteractableActor.h"
#include "Inventory/InventoryAsset.h"
#include "GenericDoor.generated.h"

UENUM(BlueprintInternalUseOnly)
enum class EOpenDirection : uint8
{
	Normal,
	Flipped,
	Dynamic
};

UCLASS(Abstract)
class FAFREVGAME_API AGenericDoor final : public AInteractableActor
{
	GENERATED_BODY()

public:

	AGenericDoor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> DoorPivot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> OpenAudio;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> CloseAudio;

	UFUNCTION(BlueprintCallable, Category = Door)
		void SetOpened(const bool bInOpened, const bool bImmediate = false);

	UFUNCTION(BlueprintPure, Category = Door)
		bool IsOpened() const { return bOpened; }

	UFUNCTION(BlueprintPure, Category = Door)
		bool IsLocked() const { return KeyAsset && !bOpened; }

protected:
	
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bStartOpened;
	
	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UInventoryAsset> KeyAsset;

	UPROPERTY(EditAnywhere, Category = Settings)
		EOpenDirection OpenDirection;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 20.0f, UIMin = 20.0f, ClampMax = 150.0f, UIMax = 120.0f))
		float OpenAngle;

	UPROPERTY(EditAnywhere, Category = Settings)
		FTransformMeshData DoorMesh;

	UPROPERTY(EditAnywhere, Category = Settings)
		FInlineFloatCurve Animation;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FText LockedLabel;

	bool bOpened;
	float DoorAngle;
	FVector2D TimeRange;
	FToroInterpFloat TimeLerp;
	TWeakObjectPtr<AActor> Interactor;
	TObjectPtr<UInventoryManager> Inventory;

	void UpdateDoorAngle() const;

	virtual bool GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info) override;
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit) override;
	virtual void OnPawnInteract_Implementation(APawn* Pawn, const FHitResult& Hit) override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
