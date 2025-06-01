// Copyright (C) RedCraft86. All Rights Reserved.

#include "LevelZones/LevelZoneVolume.h"
#include "LevelZones/LevelZoneSubsystem.h"
#include "MiscClasses/SmartCullingComponent.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroCameraManager.h"
#include "Interfaces/CharInterface.h"
#if WITH_EDITOR
#include "EngineUtils.h"
#endif

ALevelZoneVolume::ALevelZoneVolume(): CullInvert(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	PrimaryActorTick.TickInterval = 0.1f;

	ActionManager = CreateDefaultSubobject<UWorldActionComponent>("ActionManager");
	ActionManager->bManualEdFunction = true;

#if WITH_EDITOR
	bDisplayShadedVolume = true;
	BrushColor = FColor(255, 140, 0);
#endif
}

#if WITH_EDITORONLY_DATA
void ALevelZoneVolume::FindCullTargets()
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

void ALevelZoneVolume::UpdateSmartCulling()
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

void ALevelZoneVolume::BeginPlay()
{
	Super::BeginPlay();
	CamManager = AToroCameraManager::Get(this);
}

void ALevelZoneVolume::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CamManager) UpdateSmartCulling();
}

void ALevelZoneVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!IsEnabled()) return;
	if (const FGameplayTag CharTag = ICharInterface::GetCharacterID(OtherActor); VerifyCharacterTag(CharTag))
	{
		if (ULevelZoneSubsystem* Subsystem = ULevelZoneSubsystem::Get(this))
		{
			Subsystem->AssignCharToZone(CharTag, ZoneTag);
		}
		if (CharTag == Tag_Character_Player)
		{
			ActionManager->SetActions(ActionsEnter);
			ActionManager->RunActions();
		}
	}
}

void ALevelZoneVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (!IsEnabled()) return;
	if (const FGameplayTag CharTag = ICharInterface::GetCharacterID(OtherActor); CharTag == Tag_Character_Player)
	{
		ActionManager->SetActions(ActionsExit);
		ActionManager->RunActions();
	}
}

#if WITH_EDITOR
void ALevelZoneVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ActionManager->SetActions(ActionsEnter);
	ActionManager->AppendActions(ActionsExit);
	ActionManager->UpdateEdFunctions();
	if (ActorEdIcon)
	{
		ActorEdIcon->IconPath = TEXT("/ToroUtilities/Icons/LevelZoneVolume.LevelZoneVolume");
	}
}
#endif