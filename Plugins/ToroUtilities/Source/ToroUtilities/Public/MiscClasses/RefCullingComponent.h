// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RefCullingComponent.generated.h"

UCLASS(NotBlueprintable, DisplayName = "Reference Culling", ClassGroup = (Optimization), meta = (BlueprintSpawnableComponent))
class TOROUTILITIES_API URefCullingComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	URefCullingComponent();

	static URefCullingComponent* Get(const AActor* Target);

	UFUNCTION(BlueprintCallable, Category = ReferenceCulling)
		static URefCullingComponent* GetSmartCulling(const AActor* Target) { return Get(Target); }

	UPROPERTY(EditAnywhere, Category = ReferenceCulling)
		bool bAffectTicking;

	UFUNCTION(BlueprintCallable, Category = ReferenceCulling)
		void AddRenderRequest(const UObject* Object);

	UFUNCTION(BlueprintCallable, Category = ReferenceCulling)
		void RemoveRenderRequest(const UObject* Object);

	UFUNCTION(BlueprintPure, Category = ReferenceCulling)
		bool IsDisabled() const { return bDisabled; }
	
protected:
	
	UPROPERTY(EditAnywhere, Category = ReferenceCulling)
		bool bDisabled;

	UPROPERTY(EditAnywhere, Category = ReferenceCulling)
		TSet<TSoftObjectPtr<const UObject>> RenderRequests;

	UPROPERTY() bool bCachedHiddenState;
	UPROPERTY() bool bInitialTickState;
	
	void CheckRenderRequests();
	void UpdateRenderingState();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
