// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#if WITH_EDITOR
#include "Components/BillboardComponent.h"
#endif
#include "GameFramework/Actor.h"
#include "DataTypes/PostProcessTypes.h"
#include "UserSettings/ToroUserSettings.h"
#include "MasterPostProcess.generated.h"

/* Master/Global post-processing volume.
 * Ideally, only one should exist per map.
 */
UCLASS(NotBlueprintable, meta = (AllowedCategories = "PostProcessing"))
class TOROUTILITIES_API AMasterPostProcess final : public AActor
{
	GENERATED_BODY()

public:

	AMasterPostProcess();

	static AMasterPostProcess* Get(const UObject* ContextObject, const bool bCreateIfNotFound = true);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UPostProcessComponent> PostProcess;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UBillboardComponent> DebugBillboard;
#endif

	/* Advanced bloom control for game settings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ShowOnlyInnerProperties))
		FPPSettingOverrides SettingOverrides;

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
	void UpdateProbeMaterial(UMaterialInstanceDynamic* InMaterial);

private:

	UPROPERTY(Transient) TObjectPtr<UMaterialInstanceDynamic> LightProbe;
	UPROPERTY(Transient) TObjectPtr<UMaterialInstanceDynamic> Brightness;
	UMaterialInstanceDynamic* GetBrightnessBlendable(const UToroUserSettings* InSettings);
	
	void ApplySettings(const UToroUserSettings* InSettings);

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
