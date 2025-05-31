// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Narrative/GameNarrative.h"
#include "Inventory/GameInventory.h"
#include "Framework/ToroPlayerCharacter.h"
#include "GamePlayer.generated.h"

UCLASS()
class FAFREVGAME_API AGamePlayer final : public AToroPlayerCharacter
{
	GENERATED_BODY()

public:

	AGamePlayer();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> EquipmentRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UGameNarrative> Narrative;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UGameInventory> Inventory;
};
