// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InteractionInterface.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

USTRUCT(BlueprintType)
struct TORORUNTIME_API FInteractionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		TObjectPtr<AActor> Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		FInteractionInfo Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		bool bTriggered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		float Progress;

	FInteractionData() : Target(nullptr), Info({}), bTriggered(false), Progress(0.0f) {}
	void Reset()
	{
		Target = nullptr;
		Progress = 0.0f;
		Info = {};
	}
};

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInteractionComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInteractionComponent();

	UFUNCTION(BlueprintCallable, Category = Interaction)
		void SetEnabled(const bool bInEnabled);

	UFUNCTION(BlueprintPure, Category = Interaction)
		bool IsEnabled() const { return bEnabled; }

	UFUNCTION(BlueprintCallable, Category = Interaction)
		void SetInteracting(const bool bInInteracting);

	UFUNCTION(BlueprintPure, Category = Interaction)
		bool IsInteracting() const { return bInteracting; }

	UFUNCTION(BlueprintPure, Category = Interaction)
		const FInteractionData& GetCacheData() const { return InteractCache; }

	DECLARE_DELEGATE_RetVal(FHitResult, FInteractionLogic)
	FInteractionLogic InteractionLogic;

private:

	UPROPERTY() bool bEnabled;
	UPROPERTY() float HoldTime;
	UPROPERTY() bool bInteracting;
	UPROPERTY() FInteractionData InteractCache;
	UPROPERTY(Transient) TObjectPtr<AToroPlayerBase> Player;

	void CleanupInteraction();
	void HandleInteractionTick(float DeltaTime, const FHitResult& HitResult, const FInteractionInfo& InteractResult);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
};
