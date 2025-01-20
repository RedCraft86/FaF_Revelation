// Copyright (C) RedCraft86. All Rights Reserved.

#include "LevelZones/LevelZoneVolume.h"
#include "Characters/ToroCharacterBase.h"
#include "Framework/ToroPlayerCameraManager.h"
#include "Framework/ToroMusicManager.h"
#include "Characters/ToroPlayerBase.h"
#include "Framework/ToroGameMode.h"
#include "SmartCullingComponent.h"
#if WITH_EDITOR
#include "EngineUtils.h"
#endif

ALevelZoneVolume::ALevelZoneVolume() : Cull_Invert(false), OneShot_PlayOnce(true)
	, OneShot_Cooldown(1.0f), bCanPlayOneShot(true)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

#if WITH_EDITOR
void ALevelZoneVolume::FindCullTargets()
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	for (const AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (!USmartCullingComponent::Get(Actor)) continue;
		if (!Cull_FindBounded || (Cull_FindBounded
			&& EncompassesPoint(Actor->GetActorLocation())))
		{
			if (Cull_FindTag.IsNone() || Actor->ActorHasTag(Cull_FindTag))
			{
				Cull_Targets.Add(Actor);
			}
		}
	}
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}
#endif

void ALevelZoneVolume::UpdateSmartCulling()
{
	if (!CamManager) return;

	bool bShouldRender = EncompassesPoint(CamManager->GetCameraLocation());
	if (Cull_Invert) bShouldRender = !bShouldRender;

	for (auto It = Cull_Targets.CreateIterator(); It; ++It)
	{
		if (USmartCullingComponent* Comp = USmartCullingComponent::Get(It->LoadSynchronous()))
		{
			if (Comp->IsDisabled()) continue;
			bShouldRender ? Comp->AddRenderRequest(this)
				: Comp->RemoveRenderRequest(this);
		}
		else
		{
			It.RemoveCurrent();
		}
	}
}

bool ALevelZoneVolume::CanPlayOneShot() const
{
	if (!OneShot_Tag.IsValid()) return false;
	const FTimerManager& TimerManager = GetWorldTimerManager();
	return bCanPlayOneShot && !TimerManager.TimerExists(OneShotOffTimer)
		&& !TimerManager.TimerExists(OneShotCooldownTimer);
}

void ALevelZoneVolume::PlayOneShot() const
{
	if (CanPlayOneShot())
	{
		MusicManager->PlayLayer(this, OneShot_Tag);
	}
}

void ALevelZoneVolume::StopOneShot()
{
	if (CanPlayOneShot())
	{
		FTimerManager& TimerManager = GetWorldTimerManager();
		TimerManager.SetTimer(OneShotOffTimer, [this]()
		{
			MusicManager->StopLayerIfLooping(this, OneShot_Tag);
		}, 0.5f, false);

		if (OneShot_PlayOnce)
		{
			bCanPlayOneShot = false;
		}
		else TimerManager.SetTimer(OneShotCooldownTimer,
			[](){}, OneShot_Cooldown, false);
	}
}

void ALevelZoneVolume::BeginPlay()
{
	Super::BeginPlay();
	GameMode = AToroGameMode::Get(this);
	MusicManager = AToroMusicManager::Get(this);
	CamManager = AToroPlayerCameraManager::Get(this);

	GetWorldTimerManager().SetTimer(CullingTimer, this,
		&ThisClass::UpdateSmartCulling, 0.1f, true);
}

void ALevelZoneVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (!IsEnabled() || !GameMode) return;
	if (const AToroCharacterBase* Char = Cast<AToroCharacterBase>(OtherActor))
	{
		GameMode->CharacterZoneMap.Add(Char->GetCharacterID(), ZoneID);
	}

	if (const AToroPlayerBase* Player = Cast<AToroPlayerBase>(OtherActor))
	{
		UpdateSmartCulling();
		PlayOneShot();
	}
}

void ALevelZoneVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	if (!IsEnabled() && GameMode) return;
	if (const AToroPlayerBase* Player = Cast<AToroPlayerBase>(OtherActor))
	{
		UpdateSmartCulling();
		StopOneShot();
	}
}
