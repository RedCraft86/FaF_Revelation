// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "Framework/ToroPlayerCharacter.h"
#include "WorldActions/WorldActionComponent.h"
#include "TriggerField.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameFieldEvent, AToroPlayerCharacter*, Player);
DECLARE_MULTICAST_DELEGATE_OneParam(FGameFieldEventNative, AToroPlayerCharacter*);

UCLASS(NotBlueprintable, BlueprintType)
class TOROSYSTEMS_API ATriggerField final : public AToroVolume
{
	GENERATED_BODY()

public:

	ATriggerField();

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bSingleUse;

	UPROPERTY(EditAnywhere, Category = Settings, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Actions;

	UPROPERTY(BlueprintAssignable, DisplayName = "On Triggered")
	FGameFieldEvent OnTriggeredEvent;
	FGameFieldEventNative OnTriggered;

private:

	UPROPERTY(VisibleAnywhere, Category = Subobjects)
		TObjectPtr<UWorldActionComponent> ActionComponent;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
