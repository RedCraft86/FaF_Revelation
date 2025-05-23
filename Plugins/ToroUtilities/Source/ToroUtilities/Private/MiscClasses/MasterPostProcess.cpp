// Copyright (C) RedCraft86. All Rights Reserved.

#include "MiscClasses/MasterPostProcess.h"
#include "Components/PostProcessComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToroUtilities.h"
#if WITH_EDITOR
#include "Components/BillboardComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
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
	Settings.AutoExposureBias = 10.0f;
}

AMasterPostProcess* AMasterPostProcess::Get(const UObject* ContextObject, const bool bCreateIfNotFound)
{
	AMasterPostProcess* Out = nullptr;
#if WITH_EDITOR
	TArray<AActor*> PostProcesses;
	UGameplayStatics::GetAllActorsOfClass(ContextObject, StaticClass(), PostProcesses);
	if (PostProcesses.Num() > 1)
	{
		UE_LOG(LogToroUtilities, Warning, TEXT("Multiple Master Post Process actors found! Returning first one."));
		for (AActor* Actor : PostProcesses) if (Actor) UE_LOG(LogToroUtilities, Warning,
			TEXT("\t %s (%s) in level %s"), *Actor->GetActorLabel(),
			(PostProcesses.Find(Actor) == 0 ? TEXT("Returned") : TEXT("Skipped")),
			*GetNameSafe(Actor->GetLevel()));

		Out = Cast<AMasterPostProcess>(PostProcesses[0]);
	}
#endif
	if (!Out) Out = Cast<AMasterPostProcess>(UGameplayStatics::GetActorOfClass(ContextObject, StaticClass()));
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

void AMasterPostProcess::ApplySettings()
{
	// TODO
	BloomOverride.ApplyChoice(Settings, true);
	LumenOverride.ApplyChoice(Settings, 3, 3, true);
	MotionBlurOverride.ApplyChoice(Settings, 3);
	PostProcess->Settings = Settings;
}

void AMasterPostProcess::BeginPlay()
{
	Super::BeginPlay();
#if WITH_EDITOR
	TArray<AActor*> PostProcesses;
	UGameplayStatics::GetAllActorsOfClass(this, StaticClass(), PostProcesses);
	if (PostProcesses.Num() > 1)
	{
		UE_LOG(LogToroUtilities, Warning, TEXT("Multiple Master Post Process actors found! There should only be one per level."));
		for (const AActor* Actor : PostProcesses) if (Actor) UE_LOG(LogToroUtilities, Warning,
			TEXT("\t %s in level %s"), *Actor->GetActorLabel(), *GetNameSafe(Actor->GetLevel()));
	}
#endif
}

void AMasterPostProcess::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ApplySettings();
}

void AMasterPostProcess::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, CopyTarget))
	{
		if (CopyTarget == this) CopyTarget = nullptr;
	}
}
