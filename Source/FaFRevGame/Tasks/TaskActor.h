// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NarrativeDataTask.h"
#include "Interaction/InteractableActor.h"
#include "Narrative/NarrativeManager.h"
#include "TaskActor.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API ATaskActor : public AInteractableActor
{
	GENERATED_BODY()

public:

	ATaskActor();

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UNarrativeDataTask> Task;

	UPROPERTY(EditAnywhere, Category = Settings)
		FString Argument;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 1, UIMin = 1))
		uint8 Quantity;

	UFUNCTION(BlueprintCallable, Category = TaskActor)
		void MarkTaskComplete();

	UFUNCTION(BlueprintPure, Category = TaskActor)
		bool IsCompleted() const { return bCompleted; }

	UFUNCTION(BlueprintPure, Category = TaskActor)
		bool IsValidTaskActor() const { return Task && !Argument.IsEmpty() && Quantity != 0; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskCompleteBP);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Task Complete")
		FOnTaskCompleteBP OnTaskCompleteBP;

	DECLARE_MULTICAST_DELEGATE(FOnTaskComplete);
	FOnTaskComplete OnTaskComplete;

protected:

	bool bCompleted;
	TObjectPtr<UNarrativeManager> Narrative;

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
