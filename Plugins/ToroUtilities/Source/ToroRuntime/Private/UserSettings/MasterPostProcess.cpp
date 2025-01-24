// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/MasterPostProcess.h"
#include "Components/PostProcessComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToroRuntime.h"
#if WITH_EDITOR
#include "UserSettings/LightProbeBase.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"
#endif

AMasterPostProcess::AMasterPostProcess()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>("PostProcess");
	PostProcess->SetupAttachment(SceneRoot);
	PostProcess->BlendWeight = 1.0f;
	PostProcess->Priority = 1.0f;
	PostProcess->bUnbound = true;
	PostProcess->bEnabled = true;
	
#if WITH_EDITORONLY_DATA
	CREATE_DEBUG_ICON(DebugIcon);
	if (DebugIcon)
	{
		DebugIcon->IconPath = TEXT("/Engine/EditorResources/S_BoxReflectionCapture.S_BoxReflectionCapture");
	}
#endif

	Settings.bOverride_AutoExposureMethod = true;
	Settings.AutoExposureMethod = AEM_Manual;

	Settings.bOverride_AutoExposureBias = true;
	Settings.AutoExposureBias = 10.0f;
}

AMasterPostProcess* AMasterPostProcess::Get(const UObject* ContextObject)
{
#if WITH_EDITOR
	TArray<AActor*> PostProcesses;
	UGameplayStatics::GetAllActorsOfClass(ContextObject, StaticClass(), PostProcesses);
	if (PostProcesses.Num() > 1)
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("Multiple Master Post Process actors found! Getting the first one."));
		for (AActor* Actor : PostProcesses) if (Actor) UE_LOG(LogToroRuntime, Warning,
			TEXT("\t %s (%s) in level %s"), *Actor->GetActorLabel(),
			(PostProcesses.Find(Actor) == 0 ? TEXT("Returned") : TEXT("")),
			*GetNameSafe(Actor->GetLevel()));

		return Cast<AMasterPostProcess>(PostProcesses[0]);
	}
#endif
	return Cast<AMasterPostProcess>(UGameplayStatics::GetActorOfClass(ContextObject, StaticClass()));
}

#if WITH_EDITORONLY_DATA
void AMasterPostProcess::CopyFromTarget()
{
	if (CopyTarget.LoadSynchronous())
	{
		FPostProcessSettings NewSettings = Settings;
		if (const AMasterPostProcess* PPActor = Cast<AMasterPostProcess>(CopyTarget.Get()))
		{
			NewSettings = PPActor->Settings;
		}
		else if (const APostProcessVolume* PPVolume = Cast<APostProcessVolume>(CopyTarget.Get()))
		{
			NewSettings = PPVolume->Settings;
		}
		else if (const ACameraActor* CamActor = Cast<ACameraActor>(CopyTarget.Get()))
		{
			NewSettings = CamActor->GetCameraComponent()->PostProcessSettings;
		}
		else if (const UPostProcessComponent* PPComp = CopyTarget ? CopyTarget->FindComponentByClass<UPostProcessComponent>() : nullptr)
		{
			NewSettings = PPComp->Settings;
		}
		else if (const UCameraComponent* CamComp = CopyTarget ? CopyTarget->FindComponentByClass<UCameraComponent>() : nullptr)
		{
			NewSettings = CamComp->PostProcessSettings;
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
#if WITH_EDITOR
	if (FApp::IsGame()) return;
	const bool bUsingLumen = !PostProcess->Settings.bOverride_DynamicGlobalIlluminationMethod
		|| PostProcess->Settings.DynamicGlobalIlluminationMethod == EDynamicGlobalIlluminationMethod::Lumen;
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
		ApplySettings(UserSettings);
	}
	
#if WITH_EDITOR
	TArray<AActor*> PostProcesses;
	UGameplayStatics::GetAllActorsOfClass(this, StaticClass(), PostProcesses);
	if (PostProcesses.Num() > 1)
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("Multiple Master Post Process actors found!"));
		for (const AActor* Actor : PostProcesses) if (Actor) UE_LOG(LogToroRuntime, Warning,
			TEXT("\t %s in level %s"), *Actor->GetActorLabel(), *GetNameSafe(Actor->GetLevel()));
	}
#endif
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