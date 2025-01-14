// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#if WITH_EDITOR
#include "DebugIconComponent.h"
#endif
#include "GameFramework/Actor.h"
#include "DataTypes/PostProcessTypes.h"
#include "UserSettings/ToroUserSettings.h"
#include "ToroPostProcess.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API AToroPostProcess final : public AActor
{
	GENERATED_BODY()

public:

	AToroPostProcess();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UBoxComponent> SceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UPostProcessComponent> PostProcess;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UDebugIconComponent> DebugIcon;
#endif

	/* Advanced bloom control for game settings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, Interp, meta = (DisplayPriority = -1))
		FPPSettingOverrides SettingOverrides;

	/* Post process settings to use for this volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, Interp, meta = (ShowOnlyInnerProperties))
		FPostProcessSettings Settings;

	/* Priority of this volume. In the case of overlapping volumes the one with the highest priority
	* overrides the lower priority ones. The order is undefined if two or more overlapping volumes have the same priority
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = Settings)
		float Priority;

	/* World space radius around the volume that is used for blending (only if not unbound) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = Settings, Interp, meta = (ClampMin = 0.0, UIMin = 0.0, UIMax = 6000.0f))
		float BlendRadius;

	/* 0: No effect, 1: Full effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = Settings, Interp, meta = (UIMin = 0.0, UIMax = 1.0f))
		float BlendWeight;

	/* Whether this volume is enabled or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = Settings, Interp)
		bool bEnabled;

	/* Set this false to use the parent shape component as volume bounds. True affects the whole world regardless */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = Settings, Interp)
		bool bUnbound;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Tools")
		bool bPreserveExposure = true;
	
	UPROPERTY(EditAnywhere, Category = "Tools")
		TSoftObjectPtr<const AActor> CopyTarget = nullptr;
	
	UFUNCTION(CallInEditor, Category = "Tools")
		void CopyFromTarget();
#endif
private:

	UPROPERTY(Transient) TObjectPtr<UToroUserSettings> UserSettings;

	void ApplySettings();
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
