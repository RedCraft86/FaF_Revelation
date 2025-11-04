// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Player/PlayerCharacter.h"
#include "Components/SceneComponent.h"
#include "VisionConeComponent.generated.h"

UENUM(BlueprintType)
enum class EPlayerVisionState : uint8
{
	None,
	Sensed,
	Sighted
};

UENUM(BlueprintType)
enum class EPlayerDetectType : uint8
{
	Disabled,
	Enabled,
	Movement
};

USTRUCT(BlueprintType)
struct FVisionSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, ClampMax = 5000.0f, UIMax = 5000.0f))
		float Distance;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, ClampMax = 90.0f, UIMax = 90.0f))
		float ConeAngle;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f, ClampMax = 1.0f, UIMax = 1.0f))
		float PlayerSenseTime;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f, ClampMax = 1.0f, UIMax = 1.0f))
		float PlayerForgetTime;

	FVisionSettings(): Distance(2000.0f), ConeAngle(60.0f), PlayerSenseTime(0.2f), PlayerForgetTime(2.0f) {}
	FVisionSettings(const float InDistance, const float InConeAngle, const float InSenseTime, const float InForgetTime)
		: Distance(InDistance), ConeAngle(InConeAngle), PlayerSenseTime(InSenseTime), PlayerForgetTime(InForgetTime)
	{}

	static FVisionSettings Easy;
	static FVisionSettings Normal;
	static FVisionSettings Hard;
};

UCLASS(NotBlueprintable, ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class FAFREVGAME_API UVisionConeComponent final : public USceneComponent
{
	GENERATED_BODY()

public:

	UVisionConeComponent();

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ShowOnlyInnerProperties))
		FVisionSettings Settings;

	UPROPERTY(EditAnywhere, Category = Settings)
		TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditAnywhere, Category = Settings)
		EPlayerDetectType PlayerDetection;

	UFUNCTION(BlueprintPure, Category = VisionCone)
		bool IsActorSeen(const AActor* InActor) const;

	/* Only if PlayerDetection != Disabled */
	UFUNCTION(BlueprintPure, Category = VisionCone)
		EPlayerVisionState GetPlayerDetectionState() const { return PlayerState; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDetectionBP, const EPlayerVisionState, PlayerDetection);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Player Detection")
		FOnPlayerDetectionBP OnPlayerDetectionBP;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerDetection, const EPlayerVisionState);
	FOnPlayerDetection OnPlayerDetection;

private:

	FVector2D PlayerCooldown;
	EPlayerVisionState PlayerState;
	TObjectPtr<APlayerCharacter> Player;

	void SetPlayerState(const EPlayerVisionState InState);
	bool CheckDistance(const AActor* InTarget) const;
	bool CheckConeAngle(const AActor* InTarget) const;
	bool CheckLineTrace(const AActor* InTarget) const;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
