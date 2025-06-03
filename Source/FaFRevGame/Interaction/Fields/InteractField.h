// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "Player/GamePlayer.h"
#include "Components/BrushComponent.h"
#include "Interaction/InteractionInterface.h"
#include "WorldActions/WorldActionComponent.h"
#include "InteractField.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameFieldEvent, AGamePlayer*, Player);
DECLARE_MULTICAST_DELEGATE_OneParam(FGameFieldEventNative, AGamePlayer*);

UCLASS(NotBlueprintable, BlueprintType)
class FAFREVGAME_API AInteractField final : public AToroVolume, public IInteractionInterface
{
	GENERATED_BODY()

public:

	AInteractField()
	{
		ActionComponent = CreateDefaultSubobject<UWorldActionComponent>("ActionComponent");
		ActionComponent->bManualEdFunction = true;

		if (UBrushComponent* BrushComp = GetBrushComponent())
		{
			BrushComp->SetCollisionObjectType(ECC_WorldDynamic);
			BrushComp->SetCollisionResponseToAllChannels(ECR_Ignore);
			BrushComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
			BrushComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -1))
		bool bSingleUse;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -1))
		FInteractionInfo Interaction;

	UPROPERTY(EditAnywhere, Category = Settings, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Actions;

	UPROPERTY(BlueprintAssignable, DisplayName = "On Interacted")
	FGameFieldEvent OnInteractedEvent;
	FGameFieldEventNative OnInteracted;

private:

	UPROPERTY(VisibleAnywhere, Category = Subobjects)
		TObjectPtr<UWorldActionComponent> ActionComponent;

	virtual FInteractionInfo GetInteractionInfo_Implementation(const FHitResult& HitResult) override
	{
		return IsEnabled() ? Interaction : FInteractionInfo::GetEmpty();
	}
	virtual void OnBeginInteract_Implementation(AGamePlayer* Player, const FHitResult& HitResult) override
	{
		if (!IsEnabled()) return;
		OnInteracted.Broadcast(Player);
		OnInteractedEvent.Broadcast(Player);
		ActionComponent->SetActions(Actions);
		ActionComponent->RunActions();
		if (bSingleUse) SetEnabled(false);
	}
	virtual void OnConstruction(const FTransform& Transform) override
	{
		ActionComponent->SetActions(Actions);
		ActionComponent->UpdateEdFunctions();
		if (Interaction.Label.IsEmptyOrWhitespace())
		{
			Interaction.Label = NSLOCTEXT("ToroUtilities", "Interact", "Interact");
		}
	}
};
