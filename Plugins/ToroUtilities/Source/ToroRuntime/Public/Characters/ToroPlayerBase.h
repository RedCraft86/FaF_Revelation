﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "CineCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/ToroCharacterBase.h"
#include "ToroPlayerBase.generated.h"

class AToroPlayerController;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Player);

UCLASS()
class TORORUNTIME_API AToroPlayerBase : public AToroCharacterBase
{
	GENERATED_BODY()

public:
	
	AToroPlayerBase();

	UPROPERTY(VisibleDefaultsOnly, Category = Subobjects)
		TObjectPtr<UCineCameraComponent> Camera;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Player Character"))
		static EToroValidPins GetToroPlayerCharacter(AToroPlayerBase*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroPlayerBase>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroPlayerBase>
	static T* Get(const UObject* WorldContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex));
	}

	UFUNCTION(BlueprintPure, Category = Player)
		AToroPlayerController* GetPlayerController() const;

	virtual bool GetLookTarget_Implementation(FVector& Target) override;
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) override;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
