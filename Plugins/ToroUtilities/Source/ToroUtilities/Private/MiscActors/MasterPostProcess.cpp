// Copyright (C) RedCraft86. All Rights Reserved.

#include "MiscActors/MasterPostProcess.h"
#include "Components/PostProcessComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToroUtilities.h"
#include "ToroSettings.h"
#if WITH_EDITOR
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"
#endif

AMasterPostProcess::AMasterPostProcess()
{
	PrimaryActorTick.bCanEverTick = false;
	
	PostProcess = CreateDefaultSubobject<UPostProcessComponent>("PostProcess");
	SetRootComponent(PostProcess);
	PostProcess->BlendWeight = 1.0f;
	PostProcess->Priority = 1.0f;
	PostProcess->bUnbound = true;
	PostProcess->bEnabled = true;
	
#if WITH_EDITORONLY_DATA
	VisualBillboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("DebugBillboard");
	if (VisualBillboard)
	{
		VisualBillboard->bIsScreenSizeScaled = true;
		VisualBillboard->SetupAttachment(PostProcess);
		VisualBillboard->SetWorldScale3D(FVector{0.5f});
		VisualBillboard->SetSprite(LoadObject<UTexture2D>(nullptr,
			TEXT("/ToroUtilities/Icons/MasterPostProcess.MasterPostProcess")));
	}
#endif

	Settings.bOverride_AutoExposureMethod = true;
	Settings.AutoExposureMethod = AEM_Manual;
	
	Settings.bOverride_AutoExposureBias = true;
	Settings.AutoExposureBias = 10.5f;
}

AMasterPostProcess* AMasterPostProcess::Get(const UObject* ContextObject, const bool bCreateIfNotFound)
{
	AMasterPostProcess* Out = Cast<AMasterPostProcess>(UGameplayStatics::GetActorOfClass(ContextObject, StaticClass()));
	if (!Out && bCreateIfNotFound)
	{
		UE_LOG(LogToroUtilities, Warning, TEXT("Master Post Process actor not found! Attempting to spawn one."));
		UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
		Out = World ? World->SpawnActor<AMasterPostProcess>() : nullptr;
	}

	return Out;
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
		ApplySettings();
	}
}
#endif

void AMasterPostProcess::SetUDSSettings(const FUDSSettings& InSettings)
{
	if (UDSSetterObj) UDSSetterObj->SetSettings(this, InSettings);
}

void AMasterPostProcess::ApplySettings()
{
	float Brightness = 1.0f;
	// TODO

	PostProcess->Settings = Settings;

	if (UMaterialInstanceDynamic* PPM = GetLightProbeBlendable())
	{
		PostProcess->AddOrUpdateBlendable(PPM, 1.0f);
	}

	if (UMaterialInstanceDynamic* PPM = GetBrightnessBlendable())
	{
		PPM->SetScalarParameterValue(TEXT("Brightness"), Brightness);
		PostProcess->AddOrUpdateBlendable(PPM, 1.0f);
	}
}

bool AMasterPostProcess::IsUsingLumenGI() const
{
	if (PostProcess->Settings.bOverride_DynamicGlobalIlluminationMethod)
	{
		return PostProcess->Settings.DynamicGlobalIlluminationMethod == EDynamicGlobalIlluminationMethod::Lumen;
	}
	return false;
}

UMaterialInstanceDynamic* AMasterPostProcess::GetLightProbeBlendable()
{
	if (LightProbePPM) return LightProbePPM;
	if (UMaterialInterface* BaseMat = UToroSettings::Get()->LightProbePPM.LoadSynchronous())
	{
		LightProbePPM = UMaterialInstanceDynamic::Create(BaseMat, this);
		
	}
	return LightProbePPM;
}

UMaterialInstanceDynamic* AMasterPostProcess::GetBrightnessBlendable()
{
	if (BrightnessPPM) return BrightnessPPM;
	if (UMaterialInterface* BaseMat = UToroSettings::Get()->BrightnessPPM.LoadSynchronous())
	{
		BrightnessPPM = UMaterialInstanceDynamic::Create(BaseMat, this);
	}
	return BrightnessPPM;
}

void AMasterPostProcess::OnSettingUpdate(const UToroUserSettings* UserSettings)
{
	ApplySettings();
}

void AMasterPostProcess::BeginPlay()
{
	Super::BeginPlay();
#if WITH_EDITOR
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this, StaticClass(), Actors);
		if (Actors.Num() > 0 && Actors[0] != this)
		{
			UE_LOG(LogToroUtilities, Warning, TEXT(
				"Only one instance of Master Post Process can exist per world! Removing %s"), *GetActorLabel())

			K2_DestroyActor();
			return;
		}
	}
#endif
	// TODO
	if (const TSubclassOf<UUDSSetterObject> Class = UToroSettings::Get()->UDS_Setter.LoadSynchronous())
	{
		UDSSetterObj = NewObject<UUDSSetterObject>(this, Class);
	}
}

void AMasterPostProcess::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this, StaticClass(), Actors);
		if (Actors.Num() > 0 && Actors[0] != this)
		{
			UE_LOG(LogToroUtilities, Warning, TEXT(
				"Only one instance of Master Post Process can exist per world! Remove %s"), *GetActorLabel())
			return;
		}
	}
#endif
	ApplySettings();
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
