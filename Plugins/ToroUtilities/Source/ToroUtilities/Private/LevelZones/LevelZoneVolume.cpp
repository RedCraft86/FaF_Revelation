// Copyright (C) RedCraft86. All Rights Reserved.

#include "LevelZones/LevelZoneVolume.h"

#include "Actors/ToroCharacter.h"
#include "WorldActions/WorldActionComponent.h"
#include "Components/ReferenceCulling.h"
#include "Interfaces/CharInterface.h"
#include "Kismet/GameplayStatics.h"
#include "LevelZones/LevelZoneManager.h"
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
	ActionManager->bAutoConstruction = false;

#if WITH_EDITORONLY_DATA
	bDisplayShadedVolume = true;
	bRunConstructionScriptOnDrag = true;
#endif
}

#if WITH_EDITORONLY_DATA
void ALevelZoneVolume::FindCullTargets()
{
	for (const TObjectPtr<AActor> Actor : TActorRange<AActor>(GetWorld()))
	{
		if (!URefCullComponent::Get(Actor))
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

void ALevelZoneVolume::UpdateRefCulling()
{
	bool bShouldRender = EncompassesPoint(CamManager->GetCameraLocation());
	if (CullInvert) bShouldRender = !bShouldRender;

	for (auto It = CullTargets.CreateIterator(); It; ++It)
	{
		URefCullComponent* Comp = URefCullComponent::Get(It->LoadSynchronous());
		if (Comp && Comp->IsComponentTickEnabled())
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
	CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
}

void ALevelZoneVolume::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CamManager) UpdateRefCulling();
}

void ALevelZoneVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!IsEnabled()) return;
	const FGameplayTag CharTag = ICharInterface::GetCharacterID(OtherActor);
	ULevelZoneManager* ZoneSubsystem = ULevelZoneManager::Get(this);
	if (CharacterTags::IsValidTag(CharTag) && ZoneSubsystem)
	{
		ZoneSubsystem->AssignCharToZone(CharTag, ZoneTag);
	}
	// if (const AToroPlayerCharacter* Char = Cast<AToroPlayerCharacter>(OtherActor))
	// {
	// 	ActionManager->SetActions(ActionsEnter);
	// 	ActionManager->RunActions();
	// } TODO
}

void ALevelZoneVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (!IsEnabled()) return;
	// if (const AToroPlayerCharacter* Char = Cast<AToroPlayerCharacter>(OtherActor))
	// {
	// 	ActionManager->SetActions(ActionsExit);
	// 	ActionManager->RunActions();
	// } TODO
}

#if WITH_EDITOR
void ALevelZoneVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ActionManager->SetActions(ActionsEnter, false);
	ActionManager->AppendActions(ActionsExit, true);

	bDisplayShadedVolume = true;
	BrushColor = (FLinearColor::MakeRandomColor().Desaturate(0.4f) * 0.5).ToFColor(true);
	UPDATE_VISUAL_ICON(TEXT("/ToroUtilities/Icons/LevelZoneVolume.LevelZoneVolume"));
}
#endif
