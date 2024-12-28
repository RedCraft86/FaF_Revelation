// Copyright (C) RedCraft86. All Rights Reserved.

#include "ZoneCullingVolume.h"
#include "Kismet/GameplayStatics.h"
#if WITH_EDITORONLY_DATA
#include "EngineUtils.h"
#endif

AZoneCullingVolume::AZoneCullingVolume() : bInvert(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;

#if WITH_EDITORONLY_DATA
	bRunConstructionScriptOnDrag = true;
#endif
#if WITH_EDITOR
	RandomizeColor();
#endif
}

#if WITH_EDITOR
void AZoneCullingVolume::RandomizeColor()
{
	bColored = true;
	Color = FLinearColor::MakeRandomColor();
	BrushColor = Color.ToFColor(true);
	MarkComponentsRenderStateDirty();
}

void AZoneCullingVolume::FindActors()
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	for (const AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (!UZoneCullingComponent::GetZoneCullingComponent(Actor)) continue;
		if (!bWithinBounds || (bWithinBounds && EncompassesPoint(Actor->GetActorLocation())))
		{
			if (FindTag.IsNone() || Actor->ActorHasTag(FindTag))
			{
				TargetActors.Add(Actor);
			}
		}
	}
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}
#endif

void AZoneCullingVolume::BeginPlay()
{
	Super::BeginPlay();
	for (auto It = TargetActors.CreateIterator(); It; ++It)
	{
		if (UZoneCullingComponent* Comp = UZoneCullingComponent::GetZoneCullingComponent(It->LoadSynchronous()))
		{
			if (!Comp->IsDisabled()) CullingComponents.Add(Comp);
		}
	}
	
	if (!CullingComponents.IsEmpty()) CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
}

void AZoneCullingVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!CamManager) return;

	bool bShouldRender = EncompassesPoint(CamManager->GetCameraLocation());
	if (bInvert) bShouldRender = !bShouldRender;
	
	for (const TObjectPtr<UZoneCullingComponent>& Component : CullingComponents)
	{
		if (!Component || Component->IsDisabled()) continue;
		bShouldRender ? Component->AddRenderRequest(this) : Component->RemoveRenderRequest(this);
	}
}
