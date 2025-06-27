// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InteractionCache.h"
#include "InteractionWidget.h"
#include "InteractionInterface.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TOROSYSTEMS_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UInteractionComponent();

	UFUNCTION(BlueprintCallable, Category = Interaction)
		void SetEnabled(const bool bInEnabled);

	UFUNCTION(BlueprintPure, Category = Interaction)
		bool IsEnabled() const { return bEnabled; }

	UFUNCTION(BlueprintCallable, Category = Interaction)
		void SetInteracting(const bool bInInteracting, AToroPlayerCharacter* InPlayer);

	UFUNCTION(BlueprintPure, Category = Interaction)
		bool IsInteracting() const { return bInteracting; }

	DECLARE_DELEGATE_RetVal(const FHitResult&, FHandleTrace);
	FHandleTrace HandleTrace;

protected:

	UPROPERTY() bool bEnabled;
	UPROPERTY() bool bInteracting;
	UPROPERTY() FInteractionCache InteractCache;
	UPROPERTY(Transient) TObjectPtr<AToroPlayerCharacter> Player;
	UPROPERTY(Transient) TObjectPtr<UInteractionWidget> Widget;

	void UpdateWidget();
	virtual void CleanupCache();
	virtual bool CanInteract() const { return bEnabled && HandleTrace.IsBound(); }
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
