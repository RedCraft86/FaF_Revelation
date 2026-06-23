// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "ObjectiveActor.h"
#include "DataTypes/CachedGetter.h"
#include "GameStage/FaFGameState.h"
#include "Components/ActorComponent.h"
#include "SaveObjects/GameSaveObject.h"
#include "ObjectiveManager.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FObjectiveMetadata
{
	GENERATED_BODY()
	
	FObjectiveMetadata()
		: Tag(FGameplayTag::EmptyTag), bCompleted(false), Actor(nullptr)
	{}

	FObjectiveMetadata(const FGameplayTag& InTag, const bool bInCompleted, AObjectiveActor* InActor) 
		: Tag(InTag), bCompleted(bInCompleted), Actor(InActor)
	{}

	UPROPERTY()
		FGameplayTag Tag;

	UPROPERTY()
		bool bCompleted;

	UPROPERTY()
		TWeakObjectPtr<AObjectiveActor> Actor;
};

UCLASS(NotBlueprintable, BlueprintType)
class UObjectiveManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UObjectiveManager();

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = ContextObject, DisplayName = "Get Objective Manager"))
	static UObjectiveManager* Get(const UObject* ContextObject)
	{
		const AFaFGameState* GS = AFaFGameState::Get<AFaFGameState>(ContextObject);
		return IsValid(GS) ? GS->GetObjectiveManager() : nullptr;
	}

	void CompleteObjective(const FGameplayTag& Tag);
	void CompleteObjective(const AObjectiveActor* Actor);
	void StartObjectives(const FGameplayTagContainer& NewObjectives);
	const TArray<FObjectiveMetadata>& GetObjectives() const { return Objectives; }

private:

	UPROPERTY(Transient)
		TArray<FObjectiveMetadata> Objectives;

	TCachedGetter<UGameSaveObject> GameSave{[this]()
	{
		return UGameSaveObject::Get(this);
	}};
};
