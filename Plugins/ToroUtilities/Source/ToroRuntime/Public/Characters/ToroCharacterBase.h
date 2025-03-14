// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CharacterInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "VisionCone/VisionConeComponent.h"
#include "ToroCharacterBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AToroCharacterBase : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	
	AToroCharacterBase();

	UPROPERTY(VisibleDefaultsOnly, Category = Subobjects)
		TObjectPtr<UVisionConeComponent> VisionComponent;

	UFUNCTION(BlueprintPure, Category = Game)
		FGameplayTag GetCharacterID() const { return CharacterID; }

protected:

	UPROPERTY(EditAnywhere, Category = CharacterSettings, meta = (DisplayPriority = -2, Categories = "Characters"))
		FGameplayTag CharacterID;

	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) override;
};
