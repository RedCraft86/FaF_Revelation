// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "DataTypes/PostProcessTypes.h"
#include "MasterPostProcess.generated.h"

UCLASS(NotBlueprintable, meta = (AllowedCategories = "Overrides, PostProcessing", HiddenCategories = "Rendering, Actor"))
class TOROUTILITIES_API AMasterPostProcess final : public AActor
{
	GENERATED_BODY()

public:

	AMasterPostProcess();

	static AMasterPostProcess* Get(const UObject* ContextObject, const bool bCreateIfNotFound = true);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UPostProcessComponent> PostProcess;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UBillboardComponent> VisualBillboard;
#endif

	/* Advanced bloom control for game settings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Overrides)
		FPPBloomChoice BloomOverride;

	/* Advanced lumen control for game settings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Overrides)
		FPPLumenChoice LumenOverride;

	/* Advanced bloom control for game settings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Overrides)
		FPPMotionBlurChoice MotionBlurOverride;

	/* Post process settings to use for this volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PostProcessing, Interp, meta = (ShowOnlyInnerProperties))
		FPostProcessSettings Settings;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Tools)
		bool bPreserveExposure = true;
	
	UPROPERTY(EditAnywhere, Category = Tools)
		TSoftObjectPtr<const AActor> CopyTarget = nullptr;
	
	UFUNCTION(CallInEditor, Category = Tools)
		void CopyFromTarget();
#endif

	bool IsUsingLumen() const;
	UMaterialInstanceDynamic* GetLightProbeBlendable();
	UMaterialInstanceDynamic* GetBrightnessBlendable();

private:

	UPROPERTY(Transient) TObjectPtr<UMaterialInstanceDynamic> LightProbePPM;
	UPROPERTY(Transient) TObjectPtr<UMaterialInstanceDynamic> BrightnessPPM;

	void ApplySettings();
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
