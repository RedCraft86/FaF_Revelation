// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameEnemyBase.h"
#include "EnemyManager.h"

AGameEnemyBase::AGameEnemyBase(): EnemyState(EEnemyState::None), bEnabled(false)
{
	PrimaryActorTick.bCanEverTick = true;

	StateComponent = CreateDefaultSubobject<USMStateMachineComponent>("StateMachine");
}

void AGameEnemyBase::AttackPlayer()
{
	if (PlayerChar)
	{
		PlayerChar->TryJumpscare(CharacterID);
	}
}

void AGameEnemyBase::SetEnemyState(const EEnemyState InState)
{
	if (IsEnabled() && EnemyState != InState)
	{
		EnemyState = InState;
		UEnemyManager::UpdateEnemyStatus(this);
	}
}

void AGameEnemyBase::EnableAI_Implementation(const EEnemyState StartState)
{
	if (!IsEnabled())
	{
		SetEnemyState(StartState);
		StateComponent->Start();
		bEnabled = true;
	}
}

void AGameEnemyBase::DisableAI_Implementation()
{
	if (IsEnabled())
	{
		bEnabled = false;
		StateComponent->Stop();
		SetEnemyState(EEnemyState::None);
		GetCharacterMovement()->StopMovementImmediately();
	}
}

USMInstance* AGameEnemyBase::GetStateMachine() const
{
	return StateComponent->GetInstance();
}

void AGameEnemyBase::OnPlayerJumpscared() const
{
	// Not necessarily disabled but not enabled either
	StateComponent->Stop();
	GetCharacterMovement()->StopMovementImmediately();
}

void AGameEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		UEnemyManager::RegisterEnemy(this);
		PlayerChar = APlayerCharacter::Get<APlayerCharacter>(this);
		if (PlayerChar) PlayerChar->OnJumpscared.AddUObject(this, &AGameEnemyBase::OnPlayerJumpscared);
	});
}

void AGameEnemyBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEnemyManager::UnregisterEnemy(this);
	Super::EndPlay(EndPlayReason);
}

void AGameEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
