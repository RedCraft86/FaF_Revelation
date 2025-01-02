// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "MusicManager.generated.h"

/* Game State is repurposed as a Widget Manager */

UCLASS()
class TORORUNTIME_API AMusicManager : public AGameStateBase
{
	GENERATED_BODY()

public:

	AMusicManager();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Music Manager"))
		static EToroValidPins GetMusicManager(AMusicManager*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AMusicManager>& Class);

	template <typename T = AMusicManager>
	static T* Get(const UObject* WorldContextObject)
	{
		return Cast<T>(UGameplayStatics::GetGameState(WorldContextObject));
	}
};
