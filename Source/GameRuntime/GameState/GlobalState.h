// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UE5Coro.h"
#include "Framework/ToroGameState.h"
#include "GlobalState.generated.h"

class UGlobalSaveObject;

UCLASS(NotBlueprintable, BlueprintType)
class AGlobalState final : public AToroGameState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Global, meta = (Latent, LatentInfo = LatentInfo))
		FVoidCoroutine RequestSave(FLatentActionInfo LatentInfo) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Global, meta = (Latent, LatentInfo = LatentInfo))
		FVoidCoroutine RequestLoad(FLatentActionInfo LatentInfo) const;

private:

	UPROPERTY(Transient)
		TWeakObjectPtr<UGlobalSaveObject> SaveObject;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
