// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ToroGameInstance.generated.h"

UCLASS()
class TORORUNTIME_API UToroGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UToroGameInstance();

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Game Instance"))
		static EToroValidPins GetToroGameInstance(UToroGameInstance*& OutObject, const UObject* WorldContextObject, const TSubclassOf<UToroGameInstance>& Class);

	template <typename T = UToroGameInstance>
	static T* Get(const UObject* WorldContextObject)
	{
		return Cast<T>(UGameplayStatics::GetGameInstance(WorldContextObject));
	}
	
	virtual void OnWorldBeginPlay(UWorld* InWorld);
	virtual void OnWorldTick(const float DeltaSeconds) {}

	UFUNCTION(BlueprintCallable, Category = GameInstance)
		void SetUnlitViewmode(const bool bUnlit);

	UFUNCTION(BlueprintPure, Category = GameInstance)
		bool IsUnlitViewmode() const { return CachedVMI == VMI_Unlit; }

	UFUNCTION(BlueprintCallable, Category = GameInstance)
		void SetDeveloperMode(const bool bInDeveloperMode);

	UFUNCTION(BlueprintPure, Category = GameInstance)
		bool IsDeveloperMode() const { return bDeveloperMode; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDevModeChanged, bool);
	FOnDevModeChanged OnDeveloperMode;

protected:

	UPROPERTY() int32 CachedVMI;
	UPROPERTY() bool bDeveloperMode;
	
	virtual void Init() override;
};
