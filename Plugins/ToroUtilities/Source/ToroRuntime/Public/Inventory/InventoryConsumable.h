// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "InventoryConsumable.generated.h"

class UInventoryComponent;

UCLASS(Abstract)
class TORORUNTIME_API UInventoryConsumable final : public UObject
{
	GENERATED_BODY()

public:

	/** Automatically mark this object for destruction after it's been used
	* Disable this if you have latent tasks this object is supposed to handle
	* If disabled, manually run MarkFinished() once the task is finished
	*/
	UPROPERTY(EditDefaultsOnly, Category = Settings)
		bool bAutoMarkGarbage;

	UFUNCTION(BlueprintCallable, Category = Consumable)
		void MarkFinished();

	void UseConsumable(UInventoryComponent* Inventory);
	virtual UWorld* GetWorld() const override;

protected:

	UFUNCTION(BlueprintImplementableEvent)
		bool ConsumeItem(UInventoryComponent* Inventory);
};
