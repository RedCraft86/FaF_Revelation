// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "HeartBeatGameWidget.h"
#include "CharacterAI/FRCharacter.h"
#include "Components/ActorComponent.h"
#include "HeartBeatGame.generated.h"

UCLASS(ClassGroup = (Games), meta = (BlueprintSpawnableComponent))
class FAF_REV_API UHeartBeatGame : public UActorComponent
{
	GENERATED_BODY()

	friend class UHeartBeatGameWidget;

public:

	UHeartBeatGame();

	UPROPERTY(EditAnywhere, Category = "Settings")
		FText Title;

	UPROPERTY(EditAnywhere, Category = "Settings")
		FKey KeyA;

	UPROPERTY(EditAnywhere, Category = "Settings")
		FKey KeyB;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ClampMin = 1, UIMin = 1))
		uint8 MaxChances;

	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UHeartBeatGameWidget> WidgetClass;

	UFUNCTION(BlueprintCallable, Category = "HeartBeatGame")
		void StartGame(const TArray<FString> InSequence, const AFRCharacter* Enemy);

	UFUNCTION(BlueprintCallable, Category = "HeartBeatGame")
		void StartGameStr(const FString& InSequence, const AFRCharacter* Enemy);
	
	UFUNCTION(BlueprintPure, Category = "HeartBeatGame")
		bool IsPlaying() const { return Widget ? Widget->IsInGame() : false; }
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameCallback);
	UPROPERTY(BlueprintAssignable) FGameCallback OnSuccess;
	UPROPERTY(BlueprintAssignable) FGameCallback OnFail;

protected:

	FTimerHandle KeyCooldown;
	UPROPERTY(Transient) TObjectPtr<USceneComponent> LookComp;
	UPROPERTY(Transient) TObjectPtr<UHeartBeatGameWidget> Widget;
	UPROPERTY(Transient) TObjectPtr<class AFRPlayerBase> PlayerChar;
	
	void OnGameStart() const;
	void OnGameEnd(const bool bSuccess) const;
	UFUNCTION() void OnAnyKeyPressed(const FKey& InKey);
	
	virtual void BeginPlay() override;
};
