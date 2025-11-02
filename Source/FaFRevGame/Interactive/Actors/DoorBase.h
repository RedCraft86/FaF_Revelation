// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Inventory/InventoryAsset.h"
#include "Interaction/InteractableActor.h"
#include "DoorBase.generated.h"

UCLASS(Abstract, meta = (ChildCanTick = true))
class FAFREVGAME_API ADoorBase : public AInteractableActor
{
	GENERATED_BODY()

public:

	ADoorBase();

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

	UFUNCTION(BlueprintImplementableEvent)
		void OpenStateChanged(const bool bState, const bool bImmediate);
	virtual void OpenStateChanged_Implementation(const bool bState, const bool bImmediate) {}

protected:
	
	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -1))
		bool bStartOpened;
	
	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -1))
		TObjectPtr<UInventoryAsset> KeyAsset;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FText LockedLabel;

	bool bOpened;
	TWeakObjectPtr<AActor> Interactor;
	TObjectPtr<UInventoryManager> Inventory;

	virtual bool GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info) override;
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit) override;
	virtual void OnPawnInteract_Implementation(APawn* Pawn, const FHitResult& Hit) override;

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
