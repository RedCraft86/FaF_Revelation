// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/WorldActionActor.h"
#if WITH_EDITOR
#include "Components/BillboardComponent.h"
#endif

AWorldActionActor::AWorldActionActor()
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
	
	WorldActions = CreateDefaultSubobject<UWorldActionComponent>("WorldActions");

	SetCanBeDamaged(false);
}

void AWorldActionActor::RunEvents() const
{
	WorldActions->RunEvents();
}
