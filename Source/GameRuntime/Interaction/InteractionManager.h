// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "IInteractable.h"
#include "Engine/HitResult.h"
#include "Components/ActorComponent.h"
#include "InteractionManager.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class UInteractionManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInteractionManager();

	UFUNCTION(BlueprintCallable, Category = Interaction)
		void SetInteracting(const bool bInteract);

	DECLARE_DELEGATE_RetVal(const FHitResult&, FExecuteTraceLogic);
	FExecuteTraceLogic ExecuteTraceLogic;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractTargetChanged, const TWeakObjectPtr<AActor>&)
	FOnInteractTargetChanged OnTargetChanged;

private:

	bool bWantInteract;
	bool bActiveInteract;
	TWeakObjectPtr<AActor> Target;

	void SetTarget(AActor* InTarget);

	virtual void Deactivate() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
