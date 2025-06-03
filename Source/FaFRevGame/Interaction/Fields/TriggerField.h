// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InteractField.h"
#include "Actors/ToroVolume.h"
#include "TriggerField.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class FAFREVGAME_API ATriggerField final : public AToroVolume
{
	GENERATED_BODY()

public:

	ATriggerField()
	{
		ActionComponent = CreateDefaultSubobject<UWorldActionComponent>("ActionComponent");
		ActionComponent->bManualEdFunction = true;

		GetBrushComponent()->SetCollisionProfileName("Trigger");
	}

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

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override
	{
		if (!IsEnabled()) return;
		if (AGamePlayer* Player = Cast<AGamePlayer>(OtherActor))
		{
			OnTriggered.Broadcast(Player);
			OnTriggeredEvent.Broadcast(Player);
			ActionComponent->SetActions(Actions);
			ActionComponent->RunActions();
			if (bSingleUse) SetEnabled(false);
		}
	}
	virtual void OnConstruction(const FTransform& Transform) override
	{
		ActionComponent->SetActions(Actions);
		ActionComponent->UpdateEdFunctions();
	}
};
