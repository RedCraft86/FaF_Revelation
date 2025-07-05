// Copyright (C) RedCraft86. All Rights Reserved.

#include "SightedEnemy.h"

ASightedEnemy::ASightedEnemy(): VisionState(EVisionState::None)
{
	PrimaryActorTick.TickInterval = 0.1f;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	VisionCone = CreateDefaultSubobject<UVisionConeComponent>("VisionCone");
	VisionCone->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	VisionCone->SetupAttachment(GetMesh());
}

void ASightedEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		PlayerChar = AGamePlayerChar::Get<AGamePlayerChar>(this);
	});
}

void ASightedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!PlayerChar) return;
	if (const EVisionState NewState = VisionCone->GetActorVisionState(PlayerChar); VisionState != NewState)
	{
		VisionState = NewState;
		OnVisionUpdate(VisionState);
	}
}

void ASightedEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!GetVisionAttachmentBone().IsNone())
	{
		VisionCone->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::KeepRelativeTransform,
			GetVisionAttachmentBone());
	}
}
