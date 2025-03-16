// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "SmartCullingComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Optimization), meta = (BlueprintSpawnableComponent))
class TOROCORE_API USmartCullingComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	USmartCullingComponent();
	
	static USmartCullingComponent* Get(const AActor* Target);
	UFUNCTION(BlueprintCallable, Category = ZoneCulling)
		static USmartCullingComponent* GetSmartCulling(const AActor* Target) { return Get(Target); }

	UPROPERTY(EditAnywhere, Category = ZoneCulling)
		bool bAffectTicking;

	UFUNCTION(BlueprintCallable, Category = ZoneCulling)
		void AddRenderRequest(const UObject* Object);

	UFUNCTION(BlueprintCallable, Category = ZoneCulling)
		void RemoveRenderRequest(const UObject* Object);

	UFUNCTION(BlueprintPure, Category = ZoneCulling)
		bool IsDisabled() const { return bDisableComponent; }
	
protected:
	
	UPROPERTY(EditAnywhere, Category = ZoneCulling)
		bool bDisableComponent;

	UPROPERTY(EditAnywhere, Category = ZoneCulling)
		TSet<TSoftObjectPtr<const UObject>> RenderRequests;

	UPROPERTY() bool bCachedHiddenState;
	UPROPERTY() bool bInitialTickState;
	
	void CheckRenderRequests();
	void UpdateRenderingState();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
