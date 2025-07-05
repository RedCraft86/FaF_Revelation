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

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float VisionCooldown;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Vision Update")
		void OnVisionUpdate(const EVisionState State);

	UFUNCTION(BlueprintNativeEvent)
		FName GetVisionAttachmentBone();
	FName GetVisionAttachmentBone_Implementation() { return FName("head"); }

	UFUNCTION(BlueprintPure, Category = AI)
		EVisionState GetVisionState() const { return VisionState; }

private:

	UPROPERTY() float VisionTimer;
	UPROPERTY() EVisionState VisionState;
	UPROPERTY() EVisionState PendingState;
	UPROPERTY(Transient) TObjectPtr<AGamePlayerChar> PlayerChar;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
