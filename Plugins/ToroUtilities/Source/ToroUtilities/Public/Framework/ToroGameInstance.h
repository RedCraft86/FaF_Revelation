// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "Helpers/ClassGetters.h"
#include "ToroGameInstance.generated.h"

class UToroConfigManager;

UCLASS()
class TOROUTILITIES_API UToroGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UToroGameInstance(): CachedVMI(VMI_Lit), bPlayerInvincible(false), bRanFirstLoads(false) {}

	GLOBAL_CLASS_GETTER(UToroGameInstance, GetGameInstance)

	virtual void OnWorldBeginPlay(UWorld* InWorld);
	virtual void OnWorldTick(const float DeltaSeconds) {}

	UFUNCTION(BlueprintCallable, Category = GameInstance)
		void SetUnlitViewmode(const bool bUnlit);

	UFUNCTION(BlueprintPure, Category = GameInstance)
		bool IsUnlitViewmode() const { return CachedVMI == VMI_Unlit; }

	UFUNCTION(BlueprintCallable, Category = GameInstance)
		void SetPlayerInvincible(const bool bInvincible);

	UFUNCTION(BlueprintPure, Category = GameInstance)
		bool IsPlayerInvincible() const { return bPlayerInvincible; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerInvincible, bool);
	FOnPlayerInvincible OnPlayerInvincible;

protected:

	UPROPERTY() int32 CachedVMI;
	UPROPERTY() bool bPlayerInvincible;
	UPROPERTY() bool bRanFirstLoads;
	UPROPERTY(Transient) TObjectPtr<UToroConfigManager> ConfigManager;

	UToroConfigManager* GetConfigManager();
	virtual void Init() override;
};