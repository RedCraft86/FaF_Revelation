﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "LightProbes/LightProbeManager.h"
#include "Framework/ToroCameraManager.h"
#include "Misc/MasterPostProcess.h"
#include "ToroUtilities.h"
#include "ToroSettings.h"
#include "EngineUtils.h"
#if WITH_EDITOR
#include "Subsystems/UnrealEditorSubsystem.h"
#endif

void ULightProbeManager::ForceProbeRecollection()
{
	TickTime = 1.0f;
	ProbePPM = nullptr;
}

void ULightProbeManager::UpdateProbes()
{
	if (!MasterPP || !ProbePPM) return;
	
	const FVector CamPos = GetCamera().GetTranslation();
	LightProbes.RemoveAll([](const TWeakObjectPtr<ALightProbe>& Elem) { return !Elem.IsValid(); });
	LightProbes.Sort([CamPos](const TWeakObjectPtr<ALightProbe>& A, const TWeakObjectPtr<ALightProbe>& B)
	{
		return FVector::Dist(A->GetActorLocation(), CamPos) < FVector::Dist(B->GetActorLocation(), CamPos);
	});
	
	for (uint8 i = 0; i < CachedMax; i++)
	{
		if (LightProbes.IsValidIndex(i) && LightProbes[i].IsValid())
		{
			LightProbes[i]->ApplyData(ProbePPM, i, CamPos);
		}
		else
		{
			ResetPPM(i);
		}
	}

	MasterPP->UpdateProbeMaterial(LightProbes.IsEmpty() ? nullptr : ProbePPM);
}

void ULightProbeManager::CollectProbes()
{
	if (!MasterPP)
	{
		if (!LightProbes.IsEmpty())
		{
			for (uint8 i = 0; i < CachedMax; i++)
			{
				ResetPPM(i);
			}
			LightProbes.Empty();
		}
		return;
	}

	LightProbes.Empty();
	const FTransform Camera = GetCamera();
	for (ALightProbe* Probe : TActorRange<ALightProbe>(GetWorld()))
	{
		if (LightProbes.Num() >= 32) break;
		if (Probe && Probe->IsRelevantProbe(Camera, bHasLumen))
		{
			LightProbes.AddUnique(Probe);
		}
	}
	
	const uint8 Max = GetIterationMax(); 
	if (!ProbePPM || CachedMax != Max)
	{
		CachedMax = Max;
		if (UMaterialInterface* Material = UToroSettings::Get()->GetProbeMaterial(LightProbes.Num()))
		{
			ProbePPM = UMaterialInstanceDynamic::Create(Material, this);
		}
	}

	UpdateProbes();
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
	return Super::IsTickable() && ((MasterPP && CamManager) || !FApp::IsGame());
}

void ULightProbeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#if WITH_EDITOR
	if (TickTime > (FApp::IsGame() ? 0.1f : 0.025f))
	{
		if (!FApp::IsGame() && !MasterPP)
			MasterPP = AMasterPostProcess::Get(this, false);
#else
	if (TickTime > 0.1f)
	{
#endif
		bHasLumen = !MasterPP || (MasterPP && MasterPP->IsUsingLumen());
		TickTime = 0.0f;
		CollectProbes();
	}
	else
	{
		TickTime += DeltaTime;
		if (!MasterPP) UpdateProbes();
	}
}

void ULightProbeManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	MasterPP = AMasterPostProcess::Get(this);
	CamManager = AToroCameraManager::Get(this);
}

bool ULightProbeManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && UToroSettings::Get()->IsUsingLightProbes();
}

#if WITH_EDITOR
void ULightProbeManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogToroUtilities, Display, TEXT("Light Probe Manager initialized by %s."), *GetNameSafe(GetWorld()));
	if (!FApp::IsGame()) MasterPP = AMasterPostProcess::Get(this, false);
}
#endif
