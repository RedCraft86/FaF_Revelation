// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "ToroGameMode.generated.h"

UCLASS()
class TOROCORE_API AToroGameMode : public AGameModeBase
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
};
