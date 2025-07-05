// Copyright (C) RedCraft86. All Rights Reserved.

#include "SightedEnemy.h"

ASightedEnemy::ASightedEnemy()
	: VisionCooldown(0.1f), VisionTimer(0.0f), VisionState(EVisionState::None), PendingState(EVisionState::None)
{
	PrimaryActorTick.TickInterval = 0.05f;
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

	PendingState = VisionCone->GetActorVisionState(PlayerChar);
	if (VisionTimer > 0.0f)
	{
		VisionTimer -= DeltaTime;
		if (VisionState == PendingState)
		{
			VisionTimer = 0.0f;
		}
		else if (VisionTimer <= 0.0f)
		{
			VisionTimer = 0.0f;
			VisionState = PendingState;
			OnVisionUpdate(VisionState);
		}
	}
	else if (VisionState != PendingState)
	{
		VisionTimer = VisionCooldown;
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
