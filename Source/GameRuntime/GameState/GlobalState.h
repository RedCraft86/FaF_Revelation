// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UE5Coro.h"
#include "DataTypes/GameplayRecord.h"
#include "Framework/ToroGameState.h"
#include "GlobalState.generated.h"

class UGameFlagManager;
class UGlobalSaveObject;

UCLASS(NotBlueprintable, BlueprintType)
class AGlobalState final : public AToroGameState
{
	GENERATED_BODY()

public:

	AGlobalState();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Global, meta = (Latent, LatentInfo = LatentInfo))
		FVoidCoroutine RequestSave(FLatentActionInfo LatentInfo) const;

	UFUNCTION(BlueprintCallable, Category = Global, meta = (Latent, LatentInfo = LatentInfo))
		FVoidCoroutine RequestLoad(FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintCallable, Category = Global)
		void AddSessionRecord(const FGameplayRecord& Record);

	UGameFlagManager* GetGlobalFlags() const { return GlobalFlags; }

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<UGameFlagManager> GlobalFlags;

	UPROPERTY(Transient)
		TWeakObjectPtr<UGlobalSaveObject> SaveObject;

	UPROPERTY(Transient)
		TSet<FGameplayRecord> SessionRecords;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
