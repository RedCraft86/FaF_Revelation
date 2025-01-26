﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "ToroCharacterBase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Character);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Friendly);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Enemy);

UCLASS(Abstract)
class TORORUNTIME_API AToroCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	
	AToroCharacterBase();

	UFUNCTION(BlueprintPure, Category = Game)
		FGameplayTag GetCharacterID() const { return CharacterID; }

protected:

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Categories = "Character"))
		FGameplayTag CharacterID;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
