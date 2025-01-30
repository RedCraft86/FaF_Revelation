// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CharacterInterface.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "VisionCone/VisionComponent.h"
#include "ToroCharacterBase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Character);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Friendly);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Enemy);

UCLASS(Abstract)
class TORORUNTIME_API AToroCharacterBase : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	
	AToroCharacterBase();

	UPROPERTY(VisibleDefaultsOnly, Category = Subobjects)
		TObjectPtr<UVisionComponent> VisionComponent;

	UFUNCTION(BlueprintPure, Category = Game)
		FGameplayTag GetCharacterID() const { return CharacterID; }

protected:

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Categories = "Character"))
		FGameplayTag CharacterID;

	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) override;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
