// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "GameFramework/Character.h"
#include "ToroPlayerCharacter.generated.h"

UCLASS()
class TORORUNTIME_API AToroPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AToroPlayerCharacter();

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Player Character"))
		static EToroValidPins GetToroPlayerCharacter(AToroPlayerCharacter*& OutObject, const UObject* WorldContextObject, const int32 PlayerIndex = 0, const TSubclassOf<AToroPlayerCharacter>& Class = nullptr);

	template <typename T = AToroPlayerCharacter>
	static T* Get(const UObject* WorldContextObject, const int32 PlayerIndex = 0);

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
