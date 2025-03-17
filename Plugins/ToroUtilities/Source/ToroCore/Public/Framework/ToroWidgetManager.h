// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "ToroWidgetManager.generated.h"

UCLASS()
class TOROCORE_API AToroWidgetManager : public APlayerState
{
	GENERATED_BODY()

public:

	AToroWidgetManager();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "ContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Widget Manager"))
	static EToroValidPins GetToroWidgetManager(AToroWidgetManager*& OutObject, const UObject* ContextObject, const TSubclassOf<AToroWidgetManager>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroWidgetManager>
	static T* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerState(ContextObject, PlayerIndex));
	}
};
