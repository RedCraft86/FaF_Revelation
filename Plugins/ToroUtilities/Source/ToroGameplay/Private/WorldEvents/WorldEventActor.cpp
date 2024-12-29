﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldEvents/WorldEventActor.h"
#if WITH_EDITOR
#include "Components/BillboardComponent.h"
#endif

AWorldEventActor::AWorldEventActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

#if WITH_EDITORONLY_DATA
	DefaultIconBillboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("DefaultIconBillboard");
	if (DefaultIconBillboard)
	{
		DefaultIconBillboard->SetSprite(LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EditorResources/S_Trigger.S_Trigger")));
		DefaultIconBillboard->SetWorldScale3D(FVector{0.5f});
		DefaultIconBillboard->bIsScreenSizeScaled = true;
		DefaultIconBillboard->bIsEditorOnly = true;
		DefaultIconBillboard->SetHiddenInGame(true);
		DefaultIconBillboard->SetIsVisualizationComponent(true);
		DefaultIconBillboard->SetupAttachment(SceneRoot);
	}
#endif
	
	WorldEvents = CreateDefaultSubobject<UWorldEventComponent>("WorldEvents");

	SetCanBeDamaged(false);
}

void AWorldEventActor::RunEvents() const
{
	WorldEvents->RunEvents();
}
