// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "Helpers/ClassGetters.h"
#include "GameHints/HintDatabase.h"
#include "Subsystems/WorldSubsystem.h"
#include "HintSubsystem.generated.h"

UCLASS()
class TOROSYSTEMS_API UHintSubsystem final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	UHintSubsystem() {}

	WORLD_SUBSYSTEM_GETTER(UHintSubsystem);
	
	UFUNCTION(BlueprintCallable, Category = HintSystem, meta = (WorldContext = "ContextObject"))
		static void QueueHint(const UObject* ContextObject, UPARAM(meta = (Categories = "Hint")) const FGameplayTag InHint);

	void ClearHints();
	void RegisterHint(const FGameplayTag& InHint);

private:

	UPROPERTY() TMap<FGameplayTag, float> Hints;

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { return GetStatID(); }
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override
	{
		return Super::ShouldCreateSubsystem(Outer) && UToroUtilSettings::Get()->IsOnGameplayMap(Outer);
	}
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override
	{
		return WorldType == EWorldType::Game || WorldType == EWorldType::PIE ||
			WorldType == EWorldType::GamePreview || WorldType == EWorldType::GameRPC;
	}
};
