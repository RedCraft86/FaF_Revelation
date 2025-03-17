// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "ToroMusicManager.generated.h"

UCLASS()
class TOROCORE_API AToroMusicManager : public AGameStateBase
{
	GENERATED_BODY()

public:

	AToroMusicManager();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> ThemeComponent;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "ContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Music Manager"))
	static EToroValidPins GetMusicManager(AToroMusicManager*& OutObject, const UObject* ContextObject, const TSubclassOf<AToroMusicManager>& Class);

	template <typename T = AToroMusicManager>
	static T* Get(const UObject* ContextObject)
	{
		return Cast<T>(UGameplayStatics::GetGameState(ContextObject));
	}
};
