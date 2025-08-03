// Copyright (C) RedCraft86. All Rights Reserved.

#include "SightedEnemy.h"
#include "UserSettings/ToroUserSettings.h"

ASightedEnemy::ASightedEnemy(): bDetectMovementOnly(false), VisionState(EVisionState::None)
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

	FVisionConeData Vision = NormalVision;
	switch (UToroUserSettings::Get()->GetDifficulty())
	{
	case EGameDifficulty::Easy:
		Vision = EasyVision;
		break;
	case EGameDifficulty::Hard:
		Vision = HardVision;
		break;
	default: break;
	}

	VisionCone->Distance = Vision.Distance;
	VisionCone->InnerAngle = Vision.InnerAngle;
	VisionCone->OuterAngle = Vision.OuterAngle;
}

void ASightedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!PlayerChar) return;

	const EVisionState NewState = bDetectMovementOnly && !PlayerChar->IsMoving()
		? EVisionState::None : VisionCone->GetActorVisionState(PlayerChar);

	if (VisionState != NewState)
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

	VisionCone->Distance = NormalVision.Distance;
	VisionCone->InnerAngle = NormalVision.InnerAngle;
	VisionCone->OuterAngle = NormalVision.OuterAngle;
}
