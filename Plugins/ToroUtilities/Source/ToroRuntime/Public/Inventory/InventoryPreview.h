// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "InventoryPreview.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AInventoryPreview final : public AActor
{
	GENERATED_BODY()

public:

	AInventoryPreview();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
