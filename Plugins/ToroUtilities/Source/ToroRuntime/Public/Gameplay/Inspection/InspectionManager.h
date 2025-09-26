// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InspectableActor.h"
#include "Components/SceneComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "Framework/ToroPlayerCharacter.h"
#include "InspectionManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInspectionManager : public USceneComponent
{
	GENERATED_BODY()

public:

	UInspectionManager();
	
	PLAYER_COMPONENT_GETTER(UInspectionManager, AToroPlayerCharacter, Inspection)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UInspectionManager* GetInspectionManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D TurnRate;

	UFUNCTION(BlueprintCallable, Category = Inspection)
		void BeginInspection(AInspectableActor* InInspectable);

	UFUNCTION(BlueprintCallable, Category = Inspection)
		void EndInspection();

	UFUNCTION(BlueprintCallable, Category = Inspection)
		AInspectableActor* GetInspectable() const { return Inspectable.Get(); }

	UFUNCTION(BlueprintCallable, Category = Inspection)
		bool IsInspecting() const { return Inspectable.IsValid(); }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInspection, const bool);
	FOnInspection OnInspection;

	void OnMouseXY(const FVector2D& InValue);

private:

	// TODO ui

	bool bSecretKnown;
	TObjectPtr<UInventoryManager> Inventory;
	TWeakObjectPtr<AInspectableActor> Inspectable;

	virtual void BeginPlay() override;
};
