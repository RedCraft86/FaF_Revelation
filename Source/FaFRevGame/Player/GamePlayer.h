// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Narrative/GameNarrative.h"
#include "Inventory/GameInventory.h"
#include "Framework/ToroPlayerCharacter.h"
#include "GamePlayer.generated.h"

DECLARE_GAMEPLAY_TAG_CHILD(PlayerLock, Dialogue)
DECLARE_GAMEPLAY_TAG_CHILD(PlayerLock, Inventory)
DECLARE_GAMEPLAY_TAG_CHILD(PlayerLock, Jumpscare)
DECLARE_GAMEPLAY_TAG_CHILD(PlayerLock, Hiding)
DECLARE_GAMEPLAY_TAG_CHILD(PlayerLock, Device)
DECLARE_GAMEPLAY_TAG_CHILD(PlayerLock, Guide)
DECLARE_GAMEPLAY_TAG_CHILD(PlayerLock, QTE)

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
