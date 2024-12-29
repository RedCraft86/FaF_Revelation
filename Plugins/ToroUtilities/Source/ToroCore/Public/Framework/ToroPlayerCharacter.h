// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "ToroPlayerCharacter.generated.h"

UCLASS()
class TOROCORE_API AToroPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AToroPlayerCharacter();

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Player Character"))
		static EToroValidPins GetToroPlayerCharacter(AToroPlayerCharacter*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroPlayerCharacter>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroPlayerCharacter>
	static T* Get(const UObject* WorldContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex));
	}

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
