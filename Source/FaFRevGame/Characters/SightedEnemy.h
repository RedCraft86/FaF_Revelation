// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EnemyBase.h"
#include "PlayerChar/GamePlayerChar.h"
#include "Components/VisionConeComponent.h"
#include "SightedEnemy.generated.h"

USTRUCT(BlueprintType)
struct TOROCORE_API FVisionConeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = VisionSettings, meta = (ClampMin = 10.0f, UIMin = 10.0f))
		float Distance;

	UPROPERTY(EditAnywhere, Category = VisionSettings, meta = (ClampMin = 10.0f, UIMin = 10.0f, ClampMax = 90.0f, UIMax = 90.0f))
		float OuterAngle;

	UPROPERTY(EditAnywhere, Category = VisionSettings, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float InnerAngle;
};

UCLASS(Abstract)
class FAFREVGAME_API ASightedEnemy final : public AEnemyBase
{
	GENERATED_BODY()

public:

	ASightedEnemy();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UVisionConeComponent> VisionCone;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bDetectMovementOnly;

	UPROPERTY(EditAnywhere, Category = "Settings|Vision")
		FVisionConeData EasyVision;

	UPROPERTY(EditAnywhere, Category = "Settings|Vision")
		FVisionConeData NormalVision;

	UPROPERTY(EditAnywhere, Category = "Settings|Vision")
		FVisionConeData HardVision;

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
