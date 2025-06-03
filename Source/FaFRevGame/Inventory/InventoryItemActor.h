// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroActor.h"
#include "Player/GamePlayer.h"
#include "InventoryItemData.h"
#include "Interaction/InteractionInterface.h"
#include "InventoryItemActor.generated.h"

UCLASS()
class FAFREVGAME_API AInventoryItemActor final : public AToroActor, public IInteractionInterface
{
	GENERATED_BODY()

public:

	AInventoryItemActor(): Amount(1)
	{
		Interaction.Label = INVTEXT("{Item}");
	}

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UInventoryItemData> Item;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 1, UIMin = 1))
		uint8 Amount;

	UPROPERTY(EditAnywhere, Category = Settings)
		FInteractionInfo Interaction;

private:

	bool IsArchive() const { return Item ? Item->ItemType == EInventoryItemType::Archive : false; }

	virtual FInteractionInfo GetInteractionInfo_Implementation(const FHitResult& HitResult) override
	{
		if (!Item || !IsEnabled()) return FInteractionInfo::GetEmpty();

		FInteractionInfo Result = Interaction;
		
		FFormatNamedArguments Args;
		Args.Add(TEXT("Amount"), Amount);
		Args.Add(TEXT("Item"), Item->DisplayName);
		Result.Label = FText::Format(Interaction.Label, Args);
	
		return Result;
	}

	virtual void OnBeginInteract_Implementation(AGamePlayer* Player, const FHitResult& HitResult) override
	{
		if (!IsEnabled()) return;
		Amount = Player->Inventory->AddItem(Item);
		if (Amount == 0)
		{
			SetEnabled(false);
		}
	}

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override
	{
		bool bSuper = Super::CanEditChange(InProperty);
		if (InProperty && InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(AInventoryItemActor, Amount))
		{
			return bSuper && Item ? Item->ItemType == EInventoryItemType::Generic : false;
		}
		return bSuper;
	}
#endif
};
