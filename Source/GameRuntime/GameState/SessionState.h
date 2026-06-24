// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UE5Coro.h"
#include "Framework/ToroPlayerState.h"
#include "SessionState.generated.h"

class UGameFlagManager;
class USessionSaveObject;

UCLASS(NotBlueprintable, BlueprintType)
class ASessionState final : public AToroPlayerState
{
	GENERATED_BODY()

public:

	ASessionState();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Global, meta = (Latent, LatentInfo = LatentInfo))
		FVoidCoroutine RequestSave(FLatentActionInfo LatentInfo) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Global, meta = (Latent, LatentInfo = LatentInfo))
		FVoidCoroutine RequestLoad(FLatentActionInfo LatentInfo) const;

	UGameFlagManager* GetSessionFlags() const { return SessionFlags; }

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<UGameFlagManager> SessionFlags;

	UPROPERTY(Transient)
		TWeakObjectPtr<USessionSaveObject> SaveObject;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
