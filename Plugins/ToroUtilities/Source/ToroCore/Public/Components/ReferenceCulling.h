// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "ReferenceCulling.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Optimization), meta = (BlueprintSpawnableComponent))
class TOROCORE_API URefCullComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	URefCullComponent();

	static URefCullComponent* Get(const AActor* Target);
	UFUNCTION(BlueprintPure, Category = Game, meta = (DefaultToSelf = "Target"))
		static URefCullComponent* GetReferenceCulling(const AActor* Target) { return Get(Target); }

	UPROPERTY(EditAnywhere, Category = Culling)
		bool bAffectTicking;

	UFUNCTION(BlueprintCallable, Category = Culling)
		void AddRenderRequest(const UObject* Object);

	UFUNCTION(BlueprintCallable, Category = Culling)
		void RemoveRenderRequest(const UObject* Object);

private:

	UPROPERTY() bool bCachedTickState;
	UPROPERTY(Transient) TSet<TWeakObjectPtr<const UObject>> Requests;

	void UpdateRenderingState();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
