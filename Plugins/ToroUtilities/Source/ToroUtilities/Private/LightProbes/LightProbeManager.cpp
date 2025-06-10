// Copyright (C) RedCraft86. All Rights Reserved.

#include "LightProbes/LightProbeManager.h"
#include "MiscActors/MasterPostProcess.h"
#include "ToroSettings.h"
#include "EngineUtils.h"
#if WITH_EDITOR
#include "ToroUtilities.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#endif

#define MAX_PROBES 16

void ULightProbeManager::UpdateProbes()
{
	if (UMaterialInstanceDynamic* MID = MasterPP ? MasterPP->GetLightProbeBlendable() : nullptr)
	{
		const FVector CamPos = GetCamera().GetTranslation();
		LightProbes.RemoveAll([](const TWeakObjectPtr<ALightProbeActor>& Elem) { return !Elem.IsValid(); });
		LightProbes.Sort([CamPos](const TWeakObjectPtr<ALightProbeActor>& A, const TWeakObjectPtr<ALightProbeActor>& B)
		{
			return FVector::Dist(A->GetActorLocation(), CamPos) < FVector::Dist(B->GetActorLocation(), CamPos);
		});

		for (uint8 i = 0; i < MAX_PROBES; i++)
		{
			if (LightProbes.IsValidIndex(i) && LightProbes[i].IsValid())
			{
				LightProbes[i]->ApplyData(MID, i, CamPos);
			}
			else
			{
				ResetPPM(i);
			}
		}
	}
}

void ULightProbeManager::CollectProbes()
{
	if (!MasterPP)
	{
		if (!LightProbes.IsEmpty())
		{
			for (uint8 i = 0; i < MAX_PROBES; i++)
			{
				ResetPPM(i);
			}
			LightProbes.Empty();
		}
		return;
	}

	LightProbes.Empty();
	const FTransform Camera = GetCamera();
	for (ALightProbeActor* Probe : TActorRange<ALightProbeActor>(GetWorld()))
	{
		if (Probe && Probe->IsRelevantProbe(Camera, bHasLumen))
		{
			LightProbes.AddUnique(Probe);
		}
	}

	UpdateProbes();
}

void ULightProbeManager::ResetPPM(const uint8 Idx) const
{
	if (UMaterialInstanceDynamic* MID = MasterPP ? MasterPP->GetLightProbeBlendable() : nullptr)
	{
		MID->SetVectorParameterValue(ProbeParam(Idx, false), FLinearColor::Transparent);
		MID->SetVectorParameterValue(ProbeParam(Idx, true), FLinearColor::Transparent);
	}
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
	if (TickTime > (FApp::IsGame() ? 0.25f : 0.01f))
	{
		if (!FApp::IsGame() && !MasterPP)
			MasterPP = AMasterPostProcess::Get(this, false);
#else
	if (TickTime > 0.25f)
	{
#endif
		bHasLumen = MasterPP && MasterPP->IsUsingLumenGI();
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
	CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
}

bool ULightProbeManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && !UToroSettings::Get()->LightProbePPM.IsNull();
}

#if WITH_EDITOR
void ULightProbeManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogToroUtilities, Display, TEXT("Light Probe Manager initialized by %s."), *GetNameSafe(GetWorld()));
	if (!FApp::IsGame()) MasterPP = AMasterPostProcess::Get(this, false);
}
#endif
