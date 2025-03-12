// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CharacterInterface.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "VisionCone/VisionConeComponent.h"
#include "ToroCharacterBase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Characters);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Friendly);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Enemy);

inline bool VerifyCharacterTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_Characters;
}

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
