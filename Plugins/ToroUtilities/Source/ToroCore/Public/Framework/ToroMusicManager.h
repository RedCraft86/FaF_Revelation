// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "ToroMusicManager.generated.h"

/* Game State is repurposed as a Widget Manager */

UCLASS()
class TOROCORE_API AToroMusicManager : public AGameStateBase
{
	GENERATED_BODY()

public:

	AToroMusicManager();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Music Manager"))
		static EToroValidPins GetToroMusicManager(AToroMusicManager*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroMusicManager>& Class);

	template <typename T = AToroMusicManager>
	static T* Get(const UObject* WorldContextObject)
	{
		return Cast<T>(UGameplayStatics::GetGameState(WorldContextObject));
	}
};
