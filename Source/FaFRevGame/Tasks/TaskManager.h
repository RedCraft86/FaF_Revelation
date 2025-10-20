// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#if WITH_EDITOR
#include "Components/EditorShapeComponent.h"
#endif
#include "TaskActor.h"
#include "Actors/ToroActor.h"
#include "TaskManager.generated.h"

UCLASS()
class FAFREVGAME_API ATaskManager final : public AToroActor
{
	GENERATED_BODY()

public:

	ATaskManager();

	UPROPERTY(EditAnywhere, Category = Settings)
		TSet<TObjectPtr<ATaskActor>> TaskActors;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskProgressBP, const FIntPoint&, Progress);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Task Progress")
		FOnTaskProgressBP OnTaskProgressBP;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTaskProgress, const FIntPoint&);
	FOnTaskProgress OnTaskProgress;

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UEditorShapeComponent> EditorShapes;
#endif

	void OnTaskComplete();
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
