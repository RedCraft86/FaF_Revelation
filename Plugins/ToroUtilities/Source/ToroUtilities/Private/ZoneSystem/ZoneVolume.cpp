// Copyright (C) RedCraft86. All Rights Reserved.

#include "ZoneSystem/ZoneVolume.h"
#include "Framework/ToroGameState.h"
#include "Framework/ToroCameraManager.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Interfaces/CharInterface.h"
#include "ZoneSystem/SmartCulling.h"
#if WITH_EDITOR
#include "EngineUtils.h"
#endif

DEFINE_GAMEPLAY_TAG_BASE(Zone);

AZoneVolume::AZoneVolume(): CullInvert(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	PrimaryActorTick.TickInterval = 0.1f;
}

#if WITH_EDITOR
void AZoneVolume::FindCullTargets()
{
	for (const TObjectPtr<AActor> Actor : TActorRange<AActor>(GetWorld()))
	{
		if (!USmartCullingComponent::Get(Actor))
		{
			if (CullTargets.Contains(Actor)) CullTargets.Remove(Actor);
		}
		else if (!CullFindBounded || EncompassesPoint(Actor->GetActorLocation()))
		{
			if (CullFindTag.IsNone() || Actor->ActorHasTag(CullFindTag))
			{
				CullTargets.Add(Actor);
			}
		}
	}
	
	for (auto It = CullTargets.CreateIterator(); It; ++It)
	{
		if (!It->LoadSynchronous()) It.RemoveCurrent();
	}
}
#endif

void AZoneVolume::UpdateSmartCulling()
{
	bool bShouldRender = EncompassesPoint(CamManager->GetCameraLocation());
	if (CullInvert) bShouldRender = !bShouldRender;

	for (auto It = CullTargets.CreateIterator(); It; ++It)
	{
		USmartCullingComponent* Comp = USmartCullingComponent::Get(It->LoadSynchronous());
		if (Comp && !Comp->IsDisabled())
		{
			bShouldRender ? Comp->AddRenderRequest(this) : Comp->RemoveRenderRequest(this);
		}
		else
		{
			It.RemoveCurrent();
		}
	}
}

void AZoneVolume::BeginPlay()
{
	Super::BeginPlay();
	GameState = AToroGameState::Get(this);
	CamManager = AToroCameraManager::Get(this);
}

void AZoneVolume::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CamManager) UpdateSmartCulling();
}

void AZoneVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!IsEnabled() || !GameState) return;
	if (const FGameplayTag CharTag = ICharInterface::GetCharacterID(OtherActor); VerifyCharacterTag(CharTag))
	{
		GameState->AssignCharacterToZone(CharTag, ZoneID);
		if (CharTag == Tag_Character_Player)
		{
			// TODO
		}
	}
}

void AZoneVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (!IsEnabled() || !GameState) return;
	if (const FGameplayTag CharTag = ICharInterface::GetCharacterID(OtherActor); CharTag == Tag_Character_Player)
	{
		// TODO
	}
}
