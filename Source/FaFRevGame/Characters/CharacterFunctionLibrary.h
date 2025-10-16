// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "SMInstance.h"
#include "DataTypes/PinTypes.h"
#include "Actors/ToroCharacter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CharacterFunctionLibrary.generated.h"

UCLASS()
class FAFREVGAME_API UCharacterFunctionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Logic Driver|State Machine Instances", meta = (DefaultToSelf = "Target", ExpandEnumAsExecs))
	static EToroValidPins GetOwningCharacter(USMInstance* Target, AToroCharacter*& Character)
	{
		Character = Target ? Cast<AToroCharacter>(Target->GetContext()) : nullptr;
		return Character ? EToroValidPins::Valid : EToroValidPins::NotValid;
	}
};
