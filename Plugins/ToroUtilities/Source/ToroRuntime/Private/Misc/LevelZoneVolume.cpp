// Copyright (C) RedCraft86. All Rights Reserved.

#include "Misc/LevelZoneVolume.h"
#include "Characters/ToroCharacterBase.h"
#include "Framework/ToroPlayerCameraManager.h"
#include "Framework/ToroMusicManager.h"
#include "Characters/ToroPlayerBase.h"
#include "Framework/ToroGameMode.h"
#include "SmartCullingComponent.h"
#if WITH_EDITOR
#include "EngineUtils.h"
#endif

ALevelZoneVolume::ALevelZoneVolume() : CullInvert(false), LocalSoundPlayOnce(true)
	, LocalSoundCooldown(1.0f), bCanPlayLocalSound(true)
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
		if (!USmartCullingComponent::Get(Actor))
		{
			if (CullTargets.Contains(Actor)) CullTargets.Remove(Actor);
		}
		else if (!CullFindBounded || (CullFindBounded
			&& EncompassesPoint(Actor->GetActorLocation())))
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
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}
#endif

void ALevelZoneVolume::UpdateSmartCulling()
{
	if (!CamManager) return;

	bool bShouldRender = EncompassesPoint(CamManager->GetCameraLocation());
	if (CullInvert) bShouldRender = !bShouldRender;

	for (auto It = CullTargets.CreateIterator(); It; ++It)
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

bool ALevelZoneVolume::CanPlayLocalSound() const
{
	if (!LocalSoundTag.IsValid()) return false;
	const FTimerManager& TimerManager = GetWorldTimerManager();
	return bCanPlayLocalSound && !TimerManager.TimerExists(LocalSoundOffTimer)
		&& !TimerManager.TimerExists(LocalSoundCooldownTimer);
}

void ALevelZoneVolume::PlayLocalSound() const
{
	if (CanPlayLocalSound())
	{
		MusicManager->PlayLayer(this, LocalSoundTag);
	}
}

void ALevelZoneVolume::StopLocalSound()
{
	if (CanPlayLocalSound())
	{
		FTimerManager& TimerManager = GetWorldTimerManager();
		TimerManager.SetTimer(LocalSoundOffTimer, [this]()
		{
			MusicManager->StopLayerIfLooping(this, LocalSoundTag);
		}, 0.5f, false);

		if (LocalSoundPlayOnce)
		{
			bCanPlayLocalSound = false;
		}
		else TimerManager.SetTimer(LocalSoundCooldownTimer, this,
			&ThisClass::EmptyFunc, LocalSoundCooldown, false);
	}
}

void ALevelZoneVolume::EmptyFunc()
{
	LocalSoundOffTimer.Invalidate();
	LocalSoundCooldownTimer.Invalidate();
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
		GameMode->CharacterToZone.Add(Char->GetCharacterID(), ZoneID);
	}

	if (const AToroPlayerBase* Player = Cast<AToroPlayerBase>(OtherActor))
	{
		UpdateSmartCulling();
		PlayLocalSound();
	}
}

void ALevelZoneVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	if (!IsEnabled() && GameMode) return;
	if (const AToroPlayerBase* Player = Cast<AToroPlayerBase>(OtherActor))
	{
		UpdateSmartCulling();
		StopLocalSound();
	}
}
