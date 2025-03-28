﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCore.h"
#include "ExecPinEnums.h"
#include "ToroGameInstance.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "ToroGameMode.generated.h"

UE_DECLARE_GAMEPLAY_TAG_BASE(Zone);

UCLASS()
class TOROUTILITIES_API AToroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AToroGameMode();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "ContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Game Mode"))
	static EToroValidPins GetToroGameMode(AToroGameMode*& OutObject, const UObject* ContextObject, const TSubclassOf<AToroGameMode>& Class);

	template <typename T = AToroGameMode>
	static T* Get(const UObject* ContextObject)
	{
		return Cast<T>(UGameplayStatics::GetGameMode(ContextObject));
	}

	UFUNCTION(BlueprintCallable, Category = Game)
		void AssignCharacterToZone(UPARAM(meta = (Categories = "Character")) const FGameplayTag Character, UPARAM(meta = (Categories = "Zone")) const FGameplayTag Zone);
	
protected:

	UPROPERTY(Transient) TObjectPtr<UToroGameInstance> GameInstance;
	UPROPERTY(Transient) TMap<FGameplayTag, FGameplayTag> CharacterToZone;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
