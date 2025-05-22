// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "Helpers/ClassGetters.h"
#include "ToroGameInstance.generated.h"

UCLASS()
class TOROUTILITIES_API UToroGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UToroGameInstance(): CachedVMI(VMI_Lit) {}

	GAMEPLAY_CLASS_GETTER(UToroGameInstance, GetGameInstance)

	virtual void OnWorldBeginPlay(UWorld* InWorld);
	virtual void OnWorldTick(const float DeltaSeconds) {}

	UFUNCTION(BlueprintCallable, Category = GameInstance)
		void SetUnlitViewmode(const bool bUnlit);

	UFUNCTION(BlueprintPure, Category = GameInstance)
		bool IsUnlitViewmode() const { return CachedVMI == VMI_Unlit; }

protected:

	UPROPERTY(Transient) TObjectPtr<UObject> WorldContext;
	UPROPERTY() int32 CachedVMI;

	virtual void Init() override;
};
