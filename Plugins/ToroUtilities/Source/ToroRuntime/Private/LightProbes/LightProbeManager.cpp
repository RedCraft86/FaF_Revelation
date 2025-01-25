// Copyright (C) RedCraft86. All Rights Reserved.

#include "LightProbes/LightProbeManager.h"
#include "Framework/ToroPlayerCameraManager.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "ToroRuntimeSettings.h"
#include "EngineUtils.h"
#if WITH_EDITOR
#include "Subsystems/UnrealEditorSubsystem.h"
#endif

void ULightProbeManager::UpdateProbes()
{
	const FVector CamPos = GetCamera().GetTranslation();
	LightProbes.Sort([CamPos](const TObjectPtr<ALightProbe>& A, const TObjectPtr<ALightProbe>& B)
	{
		return FVector::Dist(A->GetActorLocation(), CamPos) < FVector::Distance(B->GetActorLocation(), CamPos);
	});

	for (int32 i = 0; i < 32; i++)
	{
		if (LightProbes.IsValidIndex(i) && LightProbes[i])
		{
			LightProbes[i]->ApplyData(ProbeMPC, i);
		}
		else
		{
			ResetMPC(i);
		}
	}
}

void ULightProbeManager::CollectProbes()
{
	if (!LightProbes.IsEmpty())
	{
		LightProbes.Empty();
		for (int32 i = 0; i < 32; i++)
		{
			ResetMPC(i);
		}
	}

	if (bDisabled)
		return;
	
	LightProbes.Reserve(32);
	const FTransform Camera = GetCamera();
	for (ALightProbe* Probe : TActorRange<ALightProbe>(GetWorld()))
	{
		if (Probe && Probe->IsRelevantProbe(Camera))
			LightProbes.Add(Probe);
	}

	if (LightProbes.Num() > 32)	LightProbes.SetNum(32);
	LightProbes.RemoveAll([](const TObjectPtr<ALightProbe>& Element) { return !IsValid(Element); });
}

void ULightProbeManager::ResetMPC(const uint8 Idx) const
{
	ProbeMPC->SetScalarParameterValue(ParamName(Idx, false), 0.0f);
	ProbeMPC->SetVectorParameterValue(ParamName(Idx, true), FLinearColor::Transparent);
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
	return Super::IsTickable() && (IsValid(CamManager) || !FApp::IsGame())
		&& IsValid(MasterPP) && IsValid(ProbeMPC);
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
	if (const UMaterialParameterCollection* Collection = UToroRuntimeSettings::Get()->LightProbeMPC.LoadSynchronous())
	{
		ProbeMPC = GetWorld()->GetParameterCollectionInstance(Collection);
	}
}
