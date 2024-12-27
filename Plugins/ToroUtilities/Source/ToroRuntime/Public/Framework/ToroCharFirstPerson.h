// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "ToroCharFirstPerson.generated.h"

UCLASS()
class TORORUNTIME_API AToroCharFirstPerson : public ACharacter
{
	GENERATED_BODY()

public:
	
	AToroCharFirstPerson();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
