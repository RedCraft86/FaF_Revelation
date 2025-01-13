// Copyright (C) RedCraft86. All Rights Reserved.

#include "Misc/ToroPostProcess.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PostProcessComponent.h"

void FPPBloomChoice::ApplyChoice(FPostProcessSettings& Settings, const bool bFancy) const
{
#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		bPreviewFancy ? FancyBloom.AlterPostProcess(Settings)
			: SimpleBloom.AlterPostProcess(Settings);
	}
	else
#endif
	{
		bFancy ? FancyBloom.AlterPostProcess(Settings)
			: SimpleBloom.AlterPostProcess(Settings);
	}
}

AToroPostProcess::AToroPostProcess() : Priority(0.0f), BlendRadius(100.0f)
	, BlendWeight(1.0f), bEnabled(true), bUnbound(true)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<UBoxComponent>("SceneRoot");
	SceneRoot->SetCollisionProfileName(TEXT("NoCollision"));
	SceneRoot->SetVisibility(false);
	SetRootComponent(SceneRoot);

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>("PostProcess");
	PostProcess->SetupAttachment(SceneRoot);
	
#if WITH_EDITORONLY_DATA
	CREATE_DEBUG_ICON(DebugIcon);
	if (DebugIcon) DebugIcon->IconPath = TEXT("/Engine/EditorResources/S_BoxReflectionCapture.S_BoxReflectionCapture");
#endif

	Settings.bOverride_AutoExposureMethod = true;
	Settings.AutoExposureMethod = AEM_Manual;

	Settings.bOverride_AutoExposureBias = true;
	Settings.AutoExposureBias = 10.0f;
}

#if WITH_EDITORONLY_DATA
void AToroPostProcess::CopyFromTarget()
{
	if (CopyTarget.LoadSynchronous())
	{
		FPostProcessSettings NewSettings = Settings;
		if (const AToroPostProcess* PPActor = Cast<AToroPostProcess>(CopyTarget.LoadSynchronous()))
		{
			NewSettings = PPActor->Settings;
			Priority = PPActor->Priority;
			BlendRadius = PPActor->BlendRadius;
			BlendWeight = PPActor->BlendWeight;
			bUnbound = PPActor->bUnbound;
		}
		else if (const APostProcessVolume* PPVolume = Cast<APostProcessVolume>(CopyTarget.LoadSynchronous()))
		{
			NewSettings = PPVolume->Settings;
			Priority = PPVolume->Priority;
			BlendRadius = PPVolume->BlendRadius;
			BlendWeight = PPVolume->BlendWeight;
			bUnbound = PPVolume->bUnbound;
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
			BlendRadius = PPComp->BlendRadius;
			BlendWeight = PPComp->BlendWeight;
			bUnbound = PPComp->bUnbound;
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
		SceneRoot->SetVisibility(!bUnbound);
		ApplySettings();
	}
}
#endif

void AToroPostProcess::ApplySettings()
{
	Bloom.ApplyChoice(Settings, UserSettings->GetFancyBloom());
	PostProcess->Settings = Settings;
	PostProcess->Priority = Priority;
	PostProcess->BlendWeight = BlendWeight;
	PostProcess->BlendRadius = BlendRadius;
	PostProcess->bEnabled = bEnabled;
	PostProcess->bUnbound = bUnbound;
}

void AToroPostProcess::BeginPlay()
{
	Super::BeginPlay();
	UserSettings = UToroUserSettings::Get();
}

void AToroPostProcess::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITORONLY_DATA
	UPDATE_DEBUG_ICON(DebugIcon);
	SceneRoot->SetVisibility(!bUnbound);
#endif
	ApplySettings();
}

#if WITH_EDITOR
void AToroPostProcess::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, CopyTarget))
	{
		if (CopyTarget == this) CopyTarget = nullptr;
	}
}
#endif