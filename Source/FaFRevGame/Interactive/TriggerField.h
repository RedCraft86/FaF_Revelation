// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "WorldActions/WorldActionBase.h"
#include "Framework/ToroPlayerCharacter.h"
#include "WorldActions/WorldActionManager.h"
#include "TriggerField.generated.h"

UCLASS()
class FAFREVGAME_API ATriggerField final : public AToroVolume
{
	GENERATED_BODY()

public:

	ATriggerField();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UWorldActionManager> ActionManager;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bSingleTrigger;

	UPROPERTY(EditAnywhere, Category = Settings)
		FWorldActionArray Actions;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTriggeredBP, ATriggerField*, Target, AToroPlayerCharacter*, Player);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Triggered")
		FOnTriggeredBP OnTriggeredBP;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTriggered, ATriggerField*, AToroPlayerCharacter*)
	FOnTriggered OnTriggered;

protected:

	bool bTriggered;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
