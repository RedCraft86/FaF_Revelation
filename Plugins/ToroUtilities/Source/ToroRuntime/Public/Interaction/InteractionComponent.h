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
		FInteractionInfo Result;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		bool bTriggered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		float Progress;

	FInteractionData() : Target(nullptr), Result({}), bTriggered(false), Progress(0.0f) {}
	void Reset()
	{
		Target = nullptr;
		Progress = 0.0f;
		Result = {};
	}
};

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInteractionComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInteractionComponent();

	DECLARE_DELEGATE_RetVal(FHitResult, FInteractionLogic)
	FInteractionLogic InteractionLogic;

private:

	UPROPERTY() bool bEnabled;
	UPROPERTY() float HoldTime;
	UPROPERTY() bool bInteracting;
	UPROPERTY() FInteractionData InteractCache;
	UPROPERTY(Transient) TObjectPtr<AToroPlayerCharacter> Player;

	void CleanupInteraction();
	void HandleInteractionTick(float DeltaTime, const FHitResult& HitResult, const FInteractionInfo& InteractResult);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
