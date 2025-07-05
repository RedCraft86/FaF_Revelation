// Copyright (C) RedCraft86. All Rights Reserved.

#include "EnemyBase.h"
#include "FaFRevGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerChar/GamePlayerChar.h"
#include "EngineUtils.h"

AEnemyBase::AEnemyBase(): bJumpscare(false), EnemyState(EEnemyState::None)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	AIComponent = CreateDefaultSubobject<USMStateMachineComponent>("AIComponent");
}

void AEnemyBase::StopCharacterAI()
{	if (EnemyState != EEnemyState::None && AIComponent->IsStateMachineActive())
	{
		AIComponent->Stop();
		GetCharacterMovement()->StopMovementImmediately();
		SetEnemyState(EEnemyState::None);
	}
}

void AEnemyBase::StartCharacterAI()
{
	if (EnemyState == EEnemyState::None && !AIComponent->IsStateMachineActive())
	{
		AIComponent->Start();
		SetEnemyState(EEnemyState::Roam);
	}
}

void AEnemyBase::JumpscarePlayer()
{
	if (bJumpscare) return;
	AGamePlayerChar* Player = AGamePlayerChar::Get<AGamePlayerChar>(this);
	if (Player && Player->TryJumpscare())
	{
		bJumpscare = true;
		StopCharacterAI();
		GetGameInstance<UFaFRevGameInstance>()->LastJumpscare = GetCharacterID();
		for (const TObjectPtr<AEnemyBase> Enemy : TActorRange<AEnemyBase>(GetWorld()))
		{
			Enemy->StopCharacterAI();
		}
		OnJumpscarePlayer();
	}
	else
	{
		StartCharacterAI();
	}
}

void AEnemyBase::SetEnemyState(const EEnemyState InState)
{
	if (EnemyState != InState)
	{
		EnemyState = InState;
		if (AGamePlayerChar* Player = AGamePlayerChar::Get<AGamePlayerChar>(this))
		{
			Player->UpdateEnemyState(this);
		}
	}
}
