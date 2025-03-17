// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "ToroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ToroPlayerCharacter.generated.h"

UCLASS()
class TOROCORE_API AToroPlayerCharacter : public AToroCharacter
{
	GENERATED_BODY()

public:

	AToroPlayerCharacter();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "ContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Player Character"))
	static EToroValidPins GetToroPlayerCharacter(AToroPlayerCharacter*& OutObject, const UObject* ContextObject, const TSubclassOf<AToroPlayerCharacter>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroPlayerCharacter>
	static T* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerCharacter(ContextObject, PlayerIndex));
	}

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
