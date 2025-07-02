// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroLightBase.h"
#include "Actors/ToroActor.h"
#include "Components/BoxComponent.h"
#include "Interaction/InteractionInterface.h"
#include "WorldActions/WorldActionComponent.h"
#include "ToroLightSwitch.generated.h"

UCLASS(Abstract, meta = (ChildCanTick))
class TOROSYSTEMS_API AToroLightSwitch : public AToroActor, public IInteractionInterface
{
	GENERATED_BODY()

public:

	AToroLightSwitch();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UWorldActionComponent> ActionManager;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bSwitchState;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSet<TObjectPtr<AToroLightBase>> Lights;

	UPROPERTY(EditAnywhere, Category = Settings, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> OnActions;

	UPROPERTY(EditAnywhere, Category = Settings, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> OffActions;

	UPROPERTY(EditAnywhere, Category = Settings)
		FInteractionInfo Interaction;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Apply Switch State")
		void ApplySwitchState(bool bState) const;

	UFUNCTION(BlueprintCallable, Category = ToroSwitch)
		virtual void SetSwitchState(const bool bInState);

protected:

	virtual void UpdateLights();
	virtual void BeginPlay() override;
	virtual void EnableStateChanged(const bool bIsEnabled) override;
	virtual FInteractionInfo GetInteractInfo_Implementation(const FHitResult& HitResult) override;
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult) override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
