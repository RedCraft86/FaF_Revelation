// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "AddOns/FlowNodeAddOn.h"
#include "SaveObjects/GameSaveObject.h"
#include "GameStageRequirement.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class UGameStageRequirement final : public UFlowNodeAddOn
{
	GENERATED_BODY()

public:

	UGameStageRequirement();

	bool Initialize();
	bool IsFulfilled() const;

private:

	UPROPERTY(EditAnywhere, Category = Requirement, meta = (Categories = "Objective"))
		FGameplayTag RequiredFlag;

	UPROPERTY(Transient)
		TWeakObjectPtr<UGameSaveObject> SaveObject;

	FAsyncMessageHandle MessageHandle;
	TSharedPtr<FAsyncGameplayMessageSystem> MessageSystem;

	void OnFlagMessage(const FAsyncMessage& Message);

#if WITH_EDITOR
	virtual FText GetNodeTitle() const override { return INVTEXT("Requirement"); }
	virtual FText GetNodeConfigText() const override { return FText::FromName(RequiredFlag.GetTagName()); }
#endif
};
