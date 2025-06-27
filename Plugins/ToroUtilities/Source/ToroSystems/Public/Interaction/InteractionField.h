// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "InteractionInterface.h"
#include "MiscActors/TriggerField.h"
#include "WorldActions/WorldActionBase.h"
#include "InteractionField.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class TOROSYSTEMS_API AInteractionField final : public AToroVolume, public IInteractionInterface
{
	GENERATED_BODY()

public:

	AInteractionField();

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bSingleUse;

	UPROPERTY(EditAnywhere, Category = Settings)
		FInteractionInfo Interaction;

	UPROPERTY(EditAnywhere, Category = Settings, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Actions;

	UPROPERTY(BlueprintAssignable, DisplayName = "On Interacted")
	FGameFieldEvent OnInteractedEvent;
	FGameFieldEventNative OnInteracted;

private:

	UPROPERTY(VisibleAnywhere, Category = Subobjects)
		TObjectPtr<UWorldActionComponent> ActionComponent;

	virtual FInteractionInfo GetInteractInfo_Implementation(const FHitResult& HitResult) override;
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
