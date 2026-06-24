// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UE5Coro.h"
#include "Framework/ToroPlayerState.h"
#include "SessionState.generated.h"

class USessionSaveObject;

UCLASS(NotBlueprintable, BlueprintType)
class ASessionState final : public AToroPlayerState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Global, meta = (Latent, LatentInfo = LatentInfo))
		FVoidCoroutine RequestSave(FLatentActionInfo LatentInfo) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Global, meta = (Latent, LatentInfo = LatentInfo))
		FVoidCoroutine RequestLoad(FLatentActionInfo LatentInfo) const;

private:

	UPROPERTY(Transient)
		TWeakObjectPtr<USessionSaveObject> SaveObject;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
