// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "KeyPressGameWidget.h"
#include "Components/ActorComponent.h"
#include "KeyPressGame.generated.h"

UCLASS(ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class FAF_REV_API UKeyPressGame : public UActorComponent
{
	GENERATED_BODY()
	
	typedef TArray<FKey> FKeys;
	
public:

	UKeyPressGame();

	UPROPERTY(EditAnywhere, Category = "Settings")
		TArray<FKey> ValidKeys;

	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UKeyPressGameWidget> WidgetClass;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameCallback);
	UPROPERTY(BlueprintAssignable) FGameCallback OnSuccess;
	UPROPERTY(BlueprintAssignable) FGameCallback OnFail;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StartGame(const TArray<int32>& InRounds);
	
protected:

	int32 RoundIdx;
	TArray<FKeys> Rounds;
	UPROPERTY(Transient) TObjectPtr<UKeyPressGameWidget> Widget;
	UPROPERTY(Transient) TObjectPtr<class AFRPlayerBase> PlayerChar;

	void EndGame();
	void StartNextRound();
	UFUNCTION() void OnAnyKeyPressed(const FKey& InKey);
	
	virtual void BeginPlay() override;
};
