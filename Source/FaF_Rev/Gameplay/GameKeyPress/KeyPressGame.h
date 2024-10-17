// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "KeyPressGameWidget.h"
#include "Components/ActorComponent.h"
#include "KeyPressGame.generated.h"

UCLASS(ClassGroup = (Games), meta = (BlueprintSpawnableComponent))
class FAF_REV_API UKeyPressGame : public UActorComponent
{
	GENERATED_BODY()
	
	typedef TArray<FKey> FKeys;
	
public:

	UKeyPressGame();

	UPROPERTY(EditAnywhere, Category = "Settings")
		FText Title;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
		TArray<FKey> ValidKeys;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UKeyPressGameWidget> WidgetClass;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameCallback);
	UPROPERTY(BlueprintAssignable) FGameCallback OnSuccess;
	UPROPERTY(BlueprintAssignable) FGameCallback OnFail;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StartGame(const TArray<int32>& InRounds, const class AFRCharacter* Enemy);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StartGameStr(const FString& InRounds, const class AFRCharacter* Enemy);
	
protected:
	
	int32 RoundIdx;
	TArray<FKeys> Rounds;
	FVector2D CountdownTime;
	FTimerHandle FadeOutTimer;
	bool bGameCompleted;

	UPROPERTY(Transient) TObjectPtr<USceneComponent> LookComp;
	UPROPERTY(Transient) TObjectPtr<UKeyPressGameWidget> Widget;
	UPROPERTY(Transient) TObjectPtr<class AFRPlayerBase> PlayerChar;
	
	void EndGame();
	void FailGame();
	void StartNextRound();
	UFUNCTION() void OnAnyKeyPressed(const FKey& InKey);
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
