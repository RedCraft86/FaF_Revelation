// Copyright (C) RedCraft86. All Rights Reserved.

#include "LightProbes/LightProbeManager.h"
#include "Framework/ToroPlayerCameraManager.h"
#include "UserSettings/MasterPostProcess.h"
#include "ToroRuntimeSettings.h"
#include "EngineUtils.h"
#include "ToroRuntime.h"
#if WITH_EDITOR
#include "Subsystems/UnrealEditorSubsystem.h"
#endif

void ULightProbeManager::ForceProbeRecollection()
{
	TickTime = 1.0f;
	bNewMat = true;
}

void ULightProbeManager::UpdateProbes()
{
	if (!ProbePPM) return;
	const FVector CamPos = GetCamera().GetTranslation();
	LightProbes.Sort([CamPos](const TObjectPtr<ALightProbe>& A, const TObjectPtr<ALightProbe>& B)
	{
		return FVector::Dist(A->GetActorLocation(), CamPos) < FVector::Distance(B->GetActorLocation(), CamPos);
	});

	const uint8 Max = GetIterationMax();
	for (uint8 i = 0; i < Max; i++)
	{
		if (LightProbes.IsValidIndex(i) && LightProbes[i])
		{
			LightProbes[i]->ApplyData(ProbePPM, i);
		}
		else
		{
			ResetPPM(i);
		}
	}
}

void ULightProbeManager::CollectProbes()
{
	if (!LightProbes.IsEmpty())
	{
		const uint8 Max = GetIterationMax();
		for (uint8 i = 0; i < Max; i++)
		{
			ResetPPM(i);
		}
		LightProbes.Empty();
	}

	if (bDisabled)
	{
		if (bNewMat)
		{
			ProbePPM = nullptr;
			MasterPP->UpdateProbeMaterial(nullptr);
		}
		return;
	}
	
	LightProbes.Reserve(32);
	const FTransform Camera = GetCamera();
	for (ALightProbe* Probe : TActorRange<ALightProbe>(GetWorld()))
	{
		if (Probe && Probe->IsRelevantProbe(Camera))
			LightProbes.Add(Probe);
	}

	if (const uint8 Max = GetIterationMax(); LightProbes.Num() > Max)
	{
		LightProbes.SetNum(Max);
	}
	
	LightProbes.RemoveAll([](const TObjectPtr<ALightProbe>& Element)
	{
		return !IsValid(Element);
	});

	if (!bNewMat && ProbePPM) return;
	if (UMaterialInterface* Material = UToroRuntimeSettings::Get()->GetProbeMaterial(LightProbes.Num()))
	{
		bNewMat = false;
		ProbePPM = UMaterialInstanceDynamic::Create(Material, this);
		MasterPP->UpdateProbeMaterial(ProbePPM);
	}
}

void ULightProbeManager::ResetPPM(const uint8 Idx) const
{
	if (!ProbePPM) return;
	ProbePPM->SetVectorParameterValue(ParamName(Idx, false), FLinearColor::Transparent);
	ProbePPM->SetVectorParameterValue(ParamName(Idx, true), FLinearColor::Transparent);
}

uint8 ULightProbeManager::GetIterationMax() const
{
	const uint8 NumProbes = LightProbes.Num();
	if (NumProbes <= 8) return 8;
	if (NumProbes <= 16) return 16;
	if (NumProbes <= 24) return 24;
	return 32;
}

FTransform ULightProbeManager::GetCamera() const
{
	FVector Position;
	FRotator Rotation;
#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		if (!GEditor) return FTransform::Identity;
		if (UUnrealEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>())
		{
			Subsystem->GetLevelViewportCameraInfo(Position, Rotation);
		}
	}
	else
#endif
	{
		if (CamManager)
		{
			Position = CamManager->GetCameraLocation();
			Rotation = CamManager->GetCameraRotation();
		}
	}

	return {Rotation, Position};
}

bool ULightProbeManager::IsTickable() const
{
	return Super::IsTickable() && IsValid(MasterPP) && (IsValid(CamManager) || !FApp::IsGame());
}

void ULightProbeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TickTime > 0.1f)
	{
		CollectProbes();
		TickTime = 0.0f;
		bDisabled = MasterPP->IsUsingLumen();
	}
	else
	{
		TickTime += DeltaTime;
		if (!bDisabled) UpdateProbes();
	}
}

void ULightProbeManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	MasterPP = AMasterPostProcess::Get(this);
	CamManager = AToroPlayerCameraManager::Get(this);
}

#if WITH_EDITOR
void ULightProbeManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogToroRuntime, Display, TEXT("Light Probe Manager initialized!"));
	if (!FApp::IsGame()) MasterPP = AMasterPostProcess::Get(this);
}
#endif
