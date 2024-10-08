﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "FREnemyBase.h"
#include "FRPlayer.h"

AFREnemyBase::AFREnemyBase() : bStartRoaming(true), EnemyState(EEnemyState::None)
{
}

bool AFREnemyBase::JumpscarePlayer()
{
	if (AFRPlayerBase* Player = PlayerChar.LoadSynchronous())
	{
		if (Player->TryJumpscare())
		{
			DisableAI();
			return true;
		}
	}

	return false;
}

void AFREnemyBase::SetEnemyState(const EEnemyState InNewState)
{
	if (EnemyState != InNewState)
	{
		EnemyState = InNewState;
		if (AFRPlayerBase* Player = PlayerChar.LoadSynchronous())
		{
			if (EnemyState == EEnemyState::None || EnemyState == EEnemyState::Roam)
			{
				Player->RemoveEnemy(this);
			}
			else
			{
				Player->AddEnemy(this);
			}
		}
	}
}

void AFREnemyBase::BeginPlay()
{
	Super::BeginPlay();
	SetEnemyState(bStartRoaming ? EEnemyState::Roam : EEnemyState::None);
	if (EnemyState != EEnemyState::None && GetLogicComponent()) GetLogicComponent()->Start();
}

EEnemyState AFREnemyBase::GetHighestEnemyState(const TArray<AFREnemyBase*>& InEnemies)
{
	for (const AFREnemyBase* Enemy : InEnemies)
	{
		if (!Enemy) continue;
		switch (Enemy->GetEnemyState())
		{
		case EEnemyState::Chase: return EEnemyState::Chase;
		case EEnemyState::Search: return EEnemyState::Search;
		case EEnemyState::Alert: return EEnemyState::Alert;
		case EEnemyState::Roam: return EEnemyState::Roam;
		case EEnemyState::None: return EEnemyState::None;
		}
	}

	return EEnemyState::None;
}

AFRSightedEnemyBase::AFRSightedEnemyBase()
{
	VisionCone = CreateDefaultSubobject<UVisionConeComponent>("VisionCone");
	VisionCone->SetupAttachment(GetMesh());
}

void AFRSightedEnemyBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const FName HeadBone = GetHeadBoneName();
#if WITH_EDITOR
	if (AttachedBoneName != HeadBone)
#endif
	{
#if WITH_EDITOR
		AttachedBoneName = HeadBone;
#endif
		VisionCone->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, HeadBone);
	}
}
