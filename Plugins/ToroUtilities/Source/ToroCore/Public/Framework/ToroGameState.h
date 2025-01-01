// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "ToroGameState.generated.h"

/* Game State is repurposed as a Widget Manager */

UCLASS()
class TOROCORE_API AToroGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AToroGameState();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Game State"))
		static EToroValidPins GetToroGameState(AToroGameState*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroGameState>& Class);

	template <typename T = AToroGameState>
	static T* Get(const UObject* WorldContextObject)
	{
		return Cast<T>(UGameplayStatics::GetGameState(WorldContextObject));
	}
};
