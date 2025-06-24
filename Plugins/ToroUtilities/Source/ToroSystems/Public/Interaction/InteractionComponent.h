// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InteractionInterface.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

USTRUCT()
struct TOROSYSTEMS_API FInteractionCache
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Cache)
		bool bTriggered;

	UPROPERTY(EditAnywhere, Category = Cache)
		TObjectPtr<AActor> Target;

	UPROPERTY(EditAnywhere, Category = Cache)
		FInteractionInfo Info;

	FInteractionCache(): bTriggered(false), Target(nullptr), Info(FInteractionInfo::GetEmpty()) {}

	void Reset()
	{
		Target = nullptr;
		bTriggered = false;
		Info = FInteractionInfo::GetEmpty();
	}

	void StartInteract(AToroPlayerCharacter* Player, const FHitResult& HitResult)
	{
		if (!bTriggered)
		{
			bTriggered = true;
			IInteractionInterface::BeginInteract(Target, Player, HitResult);
		}
	}

	void StopInteract(AToroPlayerCharacter* Player)
	{
		if (bTriggered)
		{
			bTriggered = false;
			IInteractionInterface::EndInteract(Target, Player);
		}
	}
};

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

	DECLARE_DELEGATE_TwoParams(FOnDataUpdate, const bool, const FInteractionCache&);
	FOnDataUpdate OnDataUpdate;

protected:

	UPROPERTY() bool bEnabled;
	UPROPERTY() bool bInteracting;
	UPROPERTY() FInteractionCache InteractCache;
	UPROPERTY(Transient) TObjectPtr<AToroPlayerCharacter> Player;

	virtual void CleanupCache();
	virtual bool CanInteract() const { return bEnabled && HandleTrace.IsBound(); }
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
