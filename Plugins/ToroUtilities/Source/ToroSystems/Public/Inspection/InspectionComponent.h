// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InspectableActor.h"
#include "InspectionWidget.h"
#include "Framework/ToroCameraManager.h"
#include "Inventory/InventoryComponent.h"
#include "InspectionComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TOROSYSTEMS_API UInspectionComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UInspectionComponent();

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = "ContextObject"))
		static UInspectionComponent* GetInspectionComponent(const UObject* ContextObject, const int32 PlayerIndex);

	static UInspectionComponent* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		return GetInspectionComponent(ContextObject, PlayerIndex);
	}

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D TurnRate;

	UFUNCTION(BlueprintCallable, Category = Inspection)
		void BeginInspection(AInspectableActor* InInspectable);

	UFUNCTION(BlueprintCallable, Category = Inspection)
		void EndInspection();

	UFUNCTION(BlueprintCallable, Category = Inspection)
		AInspectableActor* GetInspectable() { return Inspectable; }

	UFUNCTION(BlueprintCallable, Category = Inspection)
		bool IsInspecting() const { return IsValid(Inspectable); }

	DECLARE_DELEGATE_OneParam(FOnInspection, const bool);
	FOnInspection OnInspection;

	void OnMouseXY(const FVector2D& InValue);

private:

	UPROPERTY() bool bSecretKnown;
	UPROPERTY(Transient) TObjectPtr<UInspectionWidget> Widget;
	UPROPERTY(Transient) TObjectPtr<UInventoryComponent> Inventory;
	UPROPERTY(Transient) TObjectPtr<AInspectableActor> Inspectable;
	UPROPERTY(Transient) TObjectPtr<AToroCameraManager> CamManager;

	UInspectionWidget* GetWidget();
	UInventoryComponent* GetInventory();
};
