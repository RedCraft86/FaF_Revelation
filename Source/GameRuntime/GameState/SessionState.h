// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UE5Coro.h"
#include "DataTypes/GameplayRecord.h"
#include "Framework/ToroPlayerState.h"
#include "SessionState.generated.h"

class UGameFlagManager;
class UGameStageManager;
class UInventoryManager;
class USessionSaveObject;

UCLASS(NotBlueprintable, BlueprintType)
class ASessionState final : public AToroPlayerState
{
	GENERATED_BODY()

public:

	ASessionState();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Session, meta = (Latent, LatentInfo = LatentInfo))
		FVoidCoroutine RequestSave(FLatentActionInfo LatentInfo) const;

	UFUNCTION(BlueprintCallable, Category = Session, meta = (Latent, LatentInfo = LatentInfo))
		FVoidCoroutine RequestLoad(FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintCallable, Category = Session)
		void SetGameCompleted();

	UGameFlagManager* GetSessionFlags() const { return SessionFlags; }
	UGameStageManager* GetStageManager() const { return StageManager; }
	UInventoryManager* GetInventoryManager() const { return InventoryManager; }

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<UGameFlagManager> SessionFlags;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<UGameStageManager> StageManager;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<UInventoryManager> InventoryManager;

	UPROPERTY(Transient)
		TWeakObjectPtr<USessionSaveObject> SaveObject;

	UPROPERTY(Transient)
		FGameplayRecord SessionInfo;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
