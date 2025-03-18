// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventoryItemData.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/Fields/InteractField.h"
#include "InventoryItemActor.generated.h"

UCLASS()
class TOROUTILITIES_API AInventoryItemActor : public AInteractField
{
	GENERATED_BODY()

public:

	AInventoryItemActor();

	UPROPERTY(VisibleDefaultsOnly, Category = Subobject)
		TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMesh;

	// Inject Variables to Interaction Info: {Item}, {Amount}, {m + Meta Key} 
	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Item", meta = (DisplayThumbnail = false))
		TObjectPtr<UInventoryItemData> ItemData;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 1, UIMin = 1))
		uint8 Amount;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ShowOnlyInnerProperties))
		FInventoryMetadata Metadata;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (MakeEditWidget = true))
		TArray<FTransform> MeshInstances;

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void FillAmount(const uint8 InAmount);

protected:

	virtual FInteractionInfo GetInteractionInfo_Implementation(const FHitResult& HitResult) override;
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult) override;

	virtual void SetEnabled(const bool bInEnabled) override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif
};
