// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InteractionCache.h"
#include "InteractionWidget.h"
#include "InteractionInterface.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TOROSYSTEMS_API UInteractionComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInteractionComponent();

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = "ContextObject"))
		static UInteractionComponent* GetInteractionComponent(const UObject* ContextObject, const int32 PlayerIndex);

	static UInteractionComponent* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		return GetInteractionComponent(ContextObject, PlayerIndex);
	}

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
	UPROPERTY(Transient) TObjectPtr<UInteractionWidget> WidgetPtr;

	void UpdateWidget();
	void CleanupCache();
	bool CanInteract() const { return bEnabled && HandleTrace.IsBound(); }
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
