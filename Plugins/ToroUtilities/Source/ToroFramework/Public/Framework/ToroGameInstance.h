// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroGameInstance.generated.h"

UCLASS()
class TOROFRAMEWORK_API UToroGameInstance : public UGameInstance
{
	GENERATED_BODY()

	friend class AToroGameMode;

public:

	UToroGameInstance() {}

	GLOBAL_CLASS_GETTER(UToroGameInstance, GetGameInstance)

	// TODO Dev settings

protected:

	virtual void OnWorldBeginPlay(UWorld* InWorld);
	virtual void OnWorldTick(const float DeltaSeconds) {}

	virtual void Init() override;
};
