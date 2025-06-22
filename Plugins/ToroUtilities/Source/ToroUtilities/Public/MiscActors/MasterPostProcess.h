// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "MiscObjects/UDSSetter.h"
#include "DataTypes/PostProcessTypes.h"
#include "MasterPostProcess.generated.h"

UCLASS(NotBlueprintable, meta = (AllowedCategories = "PostProcessing", HiddenCategories = "Rendering, Actor"))
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FPPBloomChoice BloomOverride;

	/* Advanced lumen control for game settings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FPPLumenChoice LumenOverride;

	/* Advanced bloom control for game settings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
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

	UFUNCTION(BlueprintPure, Category = PostProcess)
		UMaterialInstanceDynamic* FindBlendable(UMaterialInterface* InMaterial) const;

	UFUNCTION(BlueprintCallable, Category = PostProcess)
		UMaterialInstanceDynamic* FindOrAddBlendable(UMaterialInterface* InMaterial);

	UFUNCTION(BlueprintCallable, Category = PostProcess)
		void RemoveBlendable(UMaterialInterface* InMaterial);

	UFUNCTION(BlueprintCallable, Category = PostProcess, DisplayName = "Set UDS Settings")
		void SetUDSSettings(const FUDSSettings& InSettings);

	UFUNCTION(BlueprintCallable, Category = PostProcess)
		void ApplySettings();

	bool IsUsingLumenGI() const;
	UMaterialInstanceDynamic* GetBrightnessBlendable();

private:
	
	UPROPERTY(Transient) TObjectPtr<UUDSSetterObject> UDSSetterObj;
	UPROPERTY(Transient) TObjectPtr<UMaterialInstanceDynamic> BrightnessPPM;
	UPROPERTY(Transient) TMap<TObjectPtr<UMaterialInterface>, TObjectPtr<UMaterialInstanceDynamic>> Blendables;

	void OnSettingUpdate(const class UToroUserSettings* UserSettings);
	
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
