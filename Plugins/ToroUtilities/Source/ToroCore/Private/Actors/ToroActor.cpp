// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroActor.h"
#if WITH_EDITOR
#include "Components/BillboardComponent.h"
#endif

AToroActor::AToroActor() : bEnabled(true), RuntimeGuid(FGuid::NewGuid())
	, bStartWithCollisionEnabled(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

#if WITH_EDITORONLY_DATA
	DefaultIconBillboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("DefaultIconBillboard");
	if (DefaultIconBillboard)
	{
		DefaultIconBillboard->SetWorldScale3D(FVector{0.5f});
		DefaultIconBillboard->bIsScreenSizeScaled = true;
		DefaultIconBillboard->bIsEditorOnly = true;
		DefaultIconBillboard->SetVisibility(false);
		DefaultIconBillboard->SetHiddenInGame(true);
		DefaultIconBillboard->SetIsVisualizationComponent(true);
		DefaultIconBillboard->SetupAttachment(SceneRoot);
	}
#endif

	SetCanBeDamaged(false);
}

void AToroActor::SetEnabled(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		ON_ENABLE_STATE_CHANGED
	}
}

void AToroActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorEnableCollision(bStartWithCollisionEnabled);
	if (!bEnabled) ON_ENABLE_STATE_CHANGED
}

void AToroActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!RuntimeGuid.IsValid())
	{
		RuntimeGuid = FGuid::NewGuid();
	}
#if WITH_EDITORONLY_DATA
	if (DefaultIconBillboard)
	{
		TArray<USceneComponent*> Components;
		GetComponents<USceneComponent>(Components);
		const bool bVisible = Components.Num() <= 2;
		DefaultIconBillboard->SetVisibility(bVisible);
		if (bVisible)
		{
			if (!BillboardIcon.Equals(BillboardIconTexture.ToString()))
			{
				BillboardIconTexture = FSoftObjectPath(BillboardIcon);
			}
			
			DefaultIconBillboard->SetSprite(BillboardIconTexture.LoadSynchronous());
		}
	}
#endif
}

void AToroActor::OnEnableStateChanged(const bool bIsEnabled)
{
	SetActorHiddenInGame(!bIsEnabled);
	SetActorEnableCollision(bIsEnabled);
	SetActorTickEnabled(PrimaryActorTick.bStartWithTickEnabled && bIsEnabled);
}