// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "ToroCharThirdPerson.generated.h"

UCLASS()
class TORORUNTIME_API AToroCharThirdPerson : public ACharacter
{
	GENERATED_BODY()

public:
	
	AToroCharThirdPerson();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
