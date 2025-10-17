// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldActions/WorldActionBase.h"
#include "Interaction/InteractableVolume.h"
#include "WorldActions/WorldActionManager.h"
#include "InteractField.generated.h"

UCLASS()
class FAFREVGAME_API AInteractField final : public AInteractableVolume
{
	GENERATED_BODY()

public:

	AInteractField();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UWorldActionManager> ActionManager;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bSingleUse;

	UPROPERTY(EditAnywhere, Category = Settings)
		FWorldActionArray Actions;

	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit) override;

protected:

	bool bUsed;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
