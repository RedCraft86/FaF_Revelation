// Copyright (C) RedCraft86. All Rights Reserved.

#include "VisionConeComponent.h"

#define GLOBAL_DISTANCE_THRESHOLD 5000.0f

FVisionSettings FVisionSettings::Easy = FVisionSettings(1500.0f, 60.0f, 0.5f, 1.0f);
FVisionSettings FVisionSettings::Normal = FVisionSettings(2000.0f, 60.0f, 0.2f, 2.0f);
FVisionSettings FVisionSettings::Hard = FVisionSettings(2000.0f, 70.0f, 0.1f, 3.0f);

UVisionConeComponent::UVisionConeComponent(): Settings(FVisionSettings::Normal)
	, TraceChannel(ECC_Visibility), PlayerDetection(EPlayerDetectType::Enabled)
	, PlayerCooldown(0.0f, 1.0f), PlayerState(EPlayerVisionState::None)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 0.05f;
}

bool UVisionConeComponent::IsActorSeen(const AActor* InActor) const
{
	return CheckDistance(InActor) && CheckConeAngle(InActor) && CheckLineTrace(InActor);
}

void UVisionConeComponent::SetPlayerState(const EPlayerVisionState InState)
{
	if (PlayerState == InState)
	{
		PlayerState = InState;
		OnPlayerDetection.Broadcast(PlayerState);
		OnPlayerDetectionBP.Broadcast(PlayerState);
	}
}

bool UVisionConeComponent::CheckDistance(const AActor* InTarget) const
{
	return InTarget && (Settings.Distance >= GLOBAL_DISTANCE_THRESHOLD
		|| FVector::Distance(GetComponentLocation(), InTarget->GetActorLocation()) <= Settings.Distance);
}

bool UVisionConeComponent::CheckConeAngle(const AActor* InTarget) const
{
	if (InTarget)
	{
		FVector FwdVec = GetForwardVector(); FwdVec.Normalize();
		FVector LookVec = InTarget->GetActorLocation() - GetComponentLocation(); LookVec.Normalize();
		const float Angle = 180.0f / UE_PI * FMath::Acos(FVector::DotProduct(LookVec, FwdVec));
		return Angle < Settings.ConeAngle;
	}
	return false;
}

bool UVisionConeComponent::CheckLineTrace(const AActor* InTarget) const
{
	if (InTarget)
	{
		const APlayerCharacter* AsPlayer = Cast<APlayerCharacter>(InTarget);
		const FVector EndPos = AsPlayer ? AsPlayer->GetEnemyTracePoint() : InTarget->GetActorLocation();

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InTarget);
		Params.AddIgnoredActor(GetOwner());
		GetWorld()->LineTraceSingleByChannel(Hit, GetComponentLocation(), EndPos, TraceChannel, Params);
		return !Hit.bBlockingHit;
	}
	return false;
}

void UVisionConeComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (PlayerDetection != EPlayerDetectType::Disabled && !GetOwner()->IsA<APlayerCharacter>())
		{
			Player = APlayerCharacter::Get<APlayerCharacter>(this);
			SetComponentTickEnabled(true);
		}
	});
}

void UVisionConeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (!Player || Player->IsKillLocked() || PlayerDetection == EPlayerDetectType::Disabled)
	{
		return;
	}
	if (PlayerDetection != EPlayerDetectType::Movement || Player->IsMoving())
	{
		if (IsActorSeen(Player))
		{
			PlayerCooldown.Y = Settings.PlayerForgetTime;
			if (PlayerState == EPlayerVisionState::None)
			{
				PlayerCooldown.X = Settings.PlayerSenseTime;
				SetPlayerState(EPlayerVisionState::Sensed);
			}
			else if (PlayerState == EPlayerVisionState::Sensed)
			{
				PlayerCooldown.X -= DeltaTime;
				if (PlayerCooldown.X <= 0.0f)
				{
					SetPlayerState(EPlayerVisionState::Sighted);
				}
			}
		}
		else if (PlayerState != EPlayerVisionState::None)
		{
			PlayerCooldown.Y -= DeltaTime;
			if (PlayerCooldown.Y <= 0.0f)
			{
				SetPlayerState(EPlayerVisionState::None);
			}
		}
	}
}
