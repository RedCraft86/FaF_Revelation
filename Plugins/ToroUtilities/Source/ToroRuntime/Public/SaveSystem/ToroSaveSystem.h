// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveObject.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroSaveSystem.generated.h"

UCLASS()
class TORORUNTIME_API UToroSaveSystem final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UToroSaveSystem();

private:
	
	UPROPERTY(Transient) TMap<FGameplayTag, TObjectPtr<UToroSaveObject>> SaveObjects;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
