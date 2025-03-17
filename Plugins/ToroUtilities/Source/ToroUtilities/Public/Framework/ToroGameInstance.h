// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ToroGameInstance.generated.h"

UCLASS()
class TOROUTILITIES_API UToroGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UToroGameInstance() : CachedVMI(VMI_Lit), bPlayerInvisible(false) {}

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "ContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Game Instance"))
	static EToroValidPins GetToroGameInstance(UToroGameInstance*& OutObject, const UObject* ContextObject, const TSubclassOf<UToroGameInstance>& Class);

	template <typename T = UToroGameInstance>
	static T* Get(const UObject* ContextObject)
	{
		return Cast<T>(UGameplayStatics::GetGameInstance(ContextObject));
	}

	virtual void OnWorldBeginPlay(UWorld* InWorld);
	virtual void OnWorldTick(const float DeltaSeconds) {}

	UFUNCTION(BlueprintCallable, Category = GameInstance)
		void SetUnlitViewmode(const bool bUnlit);

	UFUNCTION(BlueprintPure, Category = GameInstance)
		bool IsUnlitViewmode() const { return CachedVMI == VMI_Unlit; }

	UFUNCTION(BlueprintCallable, Category = GameInstance)
		void SetPlayerInvisible(const bool bInvisible);

	UFUNCTION(BlueprintPure, Category = GameInstance)
		bool IsPlayerInvisible() const { return bPlayerInvisible; }

	bool FinishedFirstLoads() const { return bFirstLoads; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerInvisible, bool);
	FOnPlayerInvisible OnPlayerInvisible;

protected:

	UPROPERTY() int32 CachedVMI;
	UPROPERTY() bool bPlayerInvisible;
	UPROPERTY() bool bFirstLoads;

	virtual void Init() override;
};
