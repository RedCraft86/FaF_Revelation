// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InlineCurves.h"
#include "ElectricActorBase.h"
#include "ZoneCullingComponent.h"
#include "ElectricLightBase.generated.h"

UENUM(BlueprintType)
enum class EElectricLightTick : uint8
{
	// Enable and disable depending on the on/off state of the light
	WithLightState,

	// Always enabled
	AlwaysOn,

	// Always disabled (expected to have no animations play)
	AlwaysOff
};

USTRUCT(BlueprintType)
struct FElectricLightEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightEntry")
		TObjectPtr<ULightComponent> Light;

	// Key: Mesh Component | Value: Should the mesh turn invisible when off?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightEntry")
		TMap<TObjectPtr<UStaticMeshComponent>, bool> Meshes;

	UPROPERTY() FLinearColor LightColor;
	UPROPERTY() FLinearColor MeshColor;
	
	FElectricLightEntry() : LightColor(FLinearColor::Black), MeshColor(FLinearColor::Black) {}

	void SetLightColorIntensity(const FLinearColor& InValue) const;
	void ForEachMesh(const TFunctionRef<void(UStaticMeshComponent*)>& Func);
	void UpdateCaches(const float MeshMulti, const float MeshFresnel);
};

USTRUCT(BlueprintInternalUseOnly)
struct FElectricLightAnim
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = LightAnim)
		bool bEnabled;
	
	UPROPERTY(EditAnywhere, Category = LightAnim, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float PlayRate;
	
	UPROPERTY(EditAnywhere, Category = LightAnim)
		FVector2D LightRange;
	
	UPROPERTY(EditAnywhere, Category = LightAnim)
		bool bMeshRange;
	
	UPROPERTY(EditAnywhere, Category = LightAnim, meta = (EditCondition = "bMeshIntensity"))
		FVector2D MeshRange;
	
	UPROPERTY(VisibleAnywhere, Category = LightAnim)
		float AnimTime;
	
	UPROPERTY(EditAnywhere, Category = LightAnim)
		FInlineColorCurve AnimCurve;

	DECLARE_DELEGATE(FOnAnimationFinished)
	FOnAnimationFinished OnAnimFinished;
	
	FElectricLightAnim();

	void Play();
	void Stop(const bool bMarkAsDone);
	void Reset();
	
	void OnTick(const float DeltaTime);
	void ModifyValues(FLinearColor& Light, FLinearColor& Mesh) const;
	void UpdateCaches();

	bool IsPlaying() const { return bPlaying; }
	bool IsFinished() const { return bFinished; }
	
	static void SetWarmupCurve(FElectricLightAnim& InAnim);
	static void SetFlickerCurve(FElectricLightAnim& InAnim);
	static void SetBreakCurve(FElectricLightAnim& InAnim);

private:

	UPROPERTY() bool bPlaying;
	UPROPERTY() bool bFinished;
	UPROPERTY() FVector2D TimeRange;
	UPROPERTY() FVector2D AlphaRange;
};

UCLASS(Abstract)
class TORORUNTIME_API AElectricLightBase final : public AElectricActorBase
{
	GENERATED_BODY()
	
public:

	AElectricLightBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UZoneCullingComponent> ZoneCulling;

	UPROPERTY(EditAnywhere, Category = Settings)
		EElectricLightTick LightTickType;
	
	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = Multiplier, meta = (ClampMin = 0.05f, UIMin = 0.05f))
		float MeshMulti;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = Fresnel)
		float MeshFresnel;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = Warmup, AdvancedDisplay = true)
		FElectricLightAnim WarmupAnim;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = Flicker, AdvancedDisplay = true)
		FElectricLightAnim FlickerAnim;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = Break, AdvancedDisplay = true)
		FElectricLightAnim BreakAnim;

	UFUNCTION(BlueprintImplementableEvent)
		bool WantsTick() const;
	
protected:
	
	UPROPERTY() float FlickerTime;
	UPROPERTY() FVector2D FlickerValRange;
	UPROPERTY() FVector2D FlickerTimeRange;
	UPROPERTY(Transient) TArray<FElectricLightEntry> CachedEntries;

	bool ShouldTick() const;
	void UpdateLight() const;
	virtual void OnStateChanged(const bool bState) override;
	virtual void OnBreakStageChanged(const EElectricBreakStage BreakState) override;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }
#endif
};
