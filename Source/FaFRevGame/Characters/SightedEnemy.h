// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EnemyBase.h"
#include "PlayerChar/GamePlayerChar.h"
#include "Components/VisionConeComponent.h"
#include "SightedEnemy.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API ASightedEnemy final : public AEnemyBase
{
	GENERATED_BODY()

public:

	ASightedEnemy();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UVisionConeComponent> VisionCone;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Vision Update")
		void OnVisionUpdate(const EVisionState State);

	UFUNCTION(BlueprintNativeEvent)
		FName GetVisionAttachmentBone();
	FName GetVisionAttachmentBone_Implementation() { return FName("head"); }

	UFUNCTION(BlueprintPure, Category = AI)
		EVisionState GetPlayerVisionState() const { return VisionState; }

private:

	UPROPERTY() EVisionState VisionState;
	UPROPERTY(Transient) TObjectPtr<AGamePlayerChar> PlayerChar;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
