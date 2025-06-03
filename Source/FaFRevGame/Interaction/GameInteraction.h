// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InteractionInterface.h"
#include "Components/ActorComponent.h"
#include "GameInteraction.generated.h"

USTRUCT(BlueprintType)
struct FAFREVGAME_API FInteractionData
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

	FInteractionData() : Target(nullptr), Info(FInteractionInfo::GetEmpty()), bTriggered(false), Progress(0.0f) {}
	void Reset()
	{
		Target = nullptr;
		Info = FInteractionInfo::GetEmpty();
		bTriggered = false;
		Progress = 0.0f;
	}
};

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class FAFREVGAME_API UGameInteraction final : public UActorComponent
{
	GENERATED_BODY()

public:

	UGameInteraction();

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 100.0f, UIMin = 100.0f))
		float ReachDistance;

	UPROPERTY(EditAnywhere, Category = Settings)
		TEnumAsByte<ECollisionChannel> InteractTrace;

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

private:

	UPROPERTY() bool bEnabled;
	UPROPERTY() float HoldTime;
	UPROPERTY() bool bInteracting;
	UPROPERTY() FInteractionData InteractCache;
	UPROPERTY(Transient) TObjectPtr<class AGamePlayer> Player;

	FHitResult Interact() const;
	void CleanupInteractionData();
	void HandleInteractionTick(float DeltaTime, const FHitResult& HitResult, const FInteractionInfo& InteractResult);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
};
