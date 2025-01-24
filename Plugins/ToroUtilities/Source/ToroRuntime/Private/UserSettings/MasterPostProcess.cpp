// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/MasterPostProcess.h"
#include "Components/PostProcessComponent.h"
#if WITH_EDITOR
#include "UserSettings/LightProbeBase.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"
#endif

AMasterPostProcess::AMasterPostProcess() : Priority(1.0f), BlendWeight(1.0f), bEnabled(true)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>("PostProcess");
	PostProcess->SetupAttachment(SceneRoot);
	
#if WITH_EDITORONLY_DATA
	CREATE_DEBUG_ICON(DebugIcon);
	if (DebugIcon)
	{
		DebugIcon->MaxComponents = 1;
		DebugIcon->IconPath = TEXT("/Engine/EditorResources/S_BoxReflectionCapture.S_BoxReflectionCapture");
	}
#endif

	Settings.bOverride_AutoExposureMethod = true;
	Settings.AutoExposureMethod = AEM_Manual;

	Settings.bOverride_AutoExposureBias = true;
	Settings.AutoExposureBias = 10.0f;
}

#if WITH_EDITORONLY_DATA
void AMasterPostProcess::CopyFromTarget()
{
	if (CopyTarget.LoadSynchronous())
	{
		FPostProcessSettings NewSettings = Settings;
		if (const AMasterPostProcess* PPActor = Cast<AMasterPostProcess>(CopyTarget.LoadSynchronous()))
		{
			NewSettings = PPActor->Settings;
			Priority = PPActor->Priority;
			BlendWeight = PPActor->BlendWeight;
		}
		else if (const APostProcessVolume* PPVolume = Cast<APostProcessVolume>(CopyTarget.LoadSynchronous()))
		{
			NewSettings = PPVolume->Settings;
			Priority = PPVolume->Priority;
			BlendWeight = PPVolume->BlendWeight;
		}
		else if (const ACameraActor* CamActor = Cast<ACameraActor>(CopyTarget.LoadSynchronous()))
		{
			NewSettings = CamActor->GetCameraComponent()->PostProcessSettings;
			BlendWeight = CamActor->GetCameraComponent()->PostProcessBlendWeight;
		}
		else if (const UPostProcessComponent* PPComp = CopyTarget ? CopyTarget->FindComponentByClass<UPostProcessComponent>() : nullptr)
		{
			NewSettings = PPComp->Settings;
			Priority = PPComp->Priority;
			BlendWeight = PPComp->BlendWeight;
		}
		else if (const UCameraComponent* CamComp = CopyTarget ? CopyTarget->FindComponentByClass<UCameraComponent>() : nullptr)
		{
			NewSettings = CamComp->PostProcessSettings;
			BlendWeight = CamComp->PostProcessBlendWeight;
		}

		if (bPreserveExposure)
		{
			NewSettings.bOverride_AutoExposureMethod = Settings.bOverride_AutoExposureMethod;
			NewSettings.AutoExposureMethod = Settings.AutoExposureMethod;

			NewSettings.bOverride_AutoExposureBias = Settings.bOverride_AutoExposureBias;
			NewSettings.AutoExposureBias = Settings.AutoExposureBias;
		}

		CopyTarget = nullptr;
		Settings = NewSettings;
		ApplySettings(nullptr);
	}
}
#endif

void AMasterPostProcess::ApplySettings(const UToroUserSettings* InSettings)
{
	SettingOverrides.ApplyChoice(Settings, InSettings);
	PostProcess->Settings = Settings;
	PostProcess->Priority = Priority;
	PostProcess->BlendWeight = BlendWeight;
	PostProcess->bEnabled = bEnabled;
	PostProcess->bUnbound = true;
#if WITH_EDITOR
	if (FApp::IsGame()) return;
	const bool bUsingLumen = PostProcess->Settings.DynamicGlobalIlluminationMethod == EDynamicGlobalIlluminationMethod::Lumen;
	for (ALightProbeBase* Probe : TActorRange<ALightProbeBase>(GetWorld()))
	{
		if (Probe) Probe->bUsingLumen = bUsingLumen;
	}
#endif
}

void AMasterPostProcess::BeginPlay()
{
	Super::BeginPlay();
	if (UToroUserSettings* UserSettings = UToroUserSettings::Get())
	{
		UserSettings->OnDynamicSettingsChanged.AddUObject(this, &ThisClass::ApplySettings);
		UserSettings->OnSettingsApplied.AddUObject(this, &ThisClass::ApplySettings);
		ApplySettings(UserSettings);
	}
}

void AMasterPostProcess::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITORONLY_DATA
	UPDATE_DEBUG_ICON(DebugIcon);
#endif
	ApplySettings(nullptr);
}

#if WITH_EDITOR
void AMasterPostProcess::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, CopyTarget))
	{
		if (CopyTarget == this) CopyTarget = nullptr;
	}
}
#endif