// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "HeartBeatGameWidget.h"
#include "Components/ActorComponent.h"
#include "HeartBeatGame.generated.h"

UCLASS(ClassGroup = (Games), meta = (BlueprintSpawnableComponent))
class FAF_REV_API UHeartBeatGame : public UActorComponent
{
	GENERATED_BODY()

public:

	UHeartBeatGame();

	UPROPERTY(EditAnywhere, Category = "Settings")
		FKey PrimaryKey;

	UPROPERTY(EditAnywhere, Category = "Settings")
		FKey SecondaryKey;

	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UHeartBeatGameWidget> WidgetClass;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameCallback);
	UPROPERTY(BlueprintAssignable) FGameCallback OnSuccess;
	UPROPERTY(BlueprintAssignable) FGameCallback OnFail;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StartGame(const FString& Sequence, const class AFRCharacter* Enemy);

protected:

	bool bIsPlaying;
	FTimerHandle FadeOutTimer;
	UPROPERTY(Transient) TObjectPtr<USceneComponent> LookComp;
	UPROPERTY(Transient) TObjectPtr<UHeartBeatGameWidget> Widget;
	UPROPERTY(Transient) TObjectPtr<class AFRPlayerBase> PlayerChar;

	void OnGameFinished(bool bSuccess);
	virtual void BeginPlay() override;
};
