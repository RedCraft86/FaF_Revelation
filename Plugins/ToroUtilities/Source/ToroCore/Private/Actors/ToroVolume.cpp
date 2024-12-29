// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroVolume.h"
#include "Components/BrushComponent.h"
#include "Components/BillboardComponent.h"

AToroVolume::AToroVolume() : bEnabled(true), RuntimeGuid(FGuid::NewGuid())
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetBrushComponent()->SetCollisionObjectType(ECC_WorldDynamic);
	GetBrushComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

#if WITH_EDITORONLY_DATA
	DefaultIconBillboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("DefaultIconBillboard");
	if (DefaultIconBillboard)
	{
		DefaultIconBillboard->SetSprite(LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EditorResources/S_TriggerBox.S_TriggerBox")));
		DefaultIconBillboard->SetWorldScale3D(FVector{0.5f});
		DefaultIconBillboard->bIsScreenSizeScaled = true;
		DefaultIconBillboard->bIsEditorOnly = true;
		DefaultIconBillboard->SetVisibility(false);
		DefaultIconBillboard->SetHiddenInGame(true);
		DefaultIconBillboard->SetIsVisualizationComponent(true);
		DefaultIconBillboard->SetupAttachment(GetRootComponent());
	}
#endif

	SetCanBeDamaged(false);
}

void AToroVolume::SetEnabled(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		ON_ENABLE_STATE_CHANGED
	}
}

void AToroVolume::BeginPlay()
{
	Super::BeginPlay();
	if (!bEnabled) ON_ENABLE_STATE_CHANGED
#if WITH_EDITORONLY_DATA
	if (DefaultIconBillboard)
	{
		DefaultIconBillboard->DestroyComponent();
	}
#endif
}

void AToroVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!RuntimeGuid.IsValid())
	{
		RuntimeGuid = FGuid::NewGuid();
	}
#if WITH_EDITORONLY_DATA
	if (DefaultIconBillboard)
	{
		TArray<USceneComponent*> TempChildren;
		DefaultIconBillboard->GetChildrenComponents(false, TempChildren);
		for (USceneComponent* Child : TempChildren)
		{
			Child->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
#endif
}

void AToroVolume::OnEnableStateChanged(const bool bIsEnabled)
{
	SetActorHiddenInGame(!bIsEnabled);
	SetActorEnableCollision(bIsEnabled);
}
