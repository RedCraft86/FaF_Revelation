// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "SmartCullingComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Optimization), meta = (BlueprintSpawnableComponent))
class TOROUTILITIES_API USmartCullingComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	USmartCullingComponent();

	static USmartCullingComponent* Get(const AActor* Target);
	UFUNCTION(BlueprintPure, Category = Game, meta = (DefaultToSelf = "Target"))
		static USmartCullingComponent* GetSmartCulling(const AActor* Target) { return Get(Target); }

	UPROPERTY(EditAnywhere, Category = SmartCulling)
		bool bAffectTicking;

	UFUNCTION(BlueprintCallable, Category = SmartCulling)
		void AddRenderRequest(const UObject* Object);

	UFUNCTION(BlueprintCallable, Category = SmartCulling)
		void RemoveRenderRequest(const UObject* Object);

	UFUNCTION(BlueprintPure, Category = SmartCulling)
		bool IsDisabled() const { return bDisabled; }

protected:

	UPROPERTY(EditAnywhere, Category = SmartCulling)
		bool bDisabled;

	UPROPERTY(EditAnywhere, Category = SmartCulling)
		TSet<TSoftObjectPtr<const UObject>> RenderRequests;

	UPROPERTY() bool bCachedHiddenState;
	UPROPERTY() bool bInitialTickState;

	void CheckRenderRequests();
	void UpdateRenderingState();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};