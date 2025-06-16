// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InteractionInterface.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

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

	FInteractionData(): Target(nullptr), Info(FInteractionInfo::GetEmpty()), bTriggered(false) {}
	void Reset()
	{
		Target = nullptr;
		Info = FInteractionInfo::GetEmpty();
		bTriggered = false;
	}
};

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class FAFREVGAME_API UInteractionComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInteractionComponent();

	static UInteractionComponent* Get(const UObject* ContextObject);
	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = "ContextObject"))
		static UInteractionComponent* GetInventory(const UObject* ContextObject) { return Get(ContextObject); }

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 100.0f, UIMin = 100.0f))
		float ReachDistance;

	UPROPERTY(EditAnywhere, Category = Settings)
		TEnumAsByte<ECollisionChannel> InteractTrace;

	UFUNCTION(BlueprintPure, Category = Interaction)
		bool IsEnabled() const;

	UFUNCTION(BlueprintCallable, Category = Interaction)
		void SetInteracting(const bool bInInteracting);

	UFUNCTION(BlueprintPure, Category = Interaction)
		bool IsInteracting() const { return bInteracting; }

	UFUNCTION(BlueprintPure, Category = Interaction)
		const FInteractionData& GetCacheData() const { return InteractCache; }

	DECLARE_DELEGATE_TwoParams(FInteractionUpdate, const bool, const FInteractionData&);
	FInteractionUpdate OnUpdate;

private:

	UPROPERTY() bool bInteracting;
	UPROPERTY() FInteractionData InteractCache;
	UPROPERTY(Transient) TObjectPtr<AGamePlayerChar> Player;

	FHitResult Interact() const;
	void CleanupInteractionData();
	void HandleInteractionTick(const FHitResult& HitResult, const FInteractionInfo& Interaction);

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
