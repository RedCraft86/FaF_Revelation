// Copyright (C) RedCraft86. All Rights Reserved.

#include "DebugIconComponent.h"

UDebugIconComponent::UDebugIconComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsScreenSizeScaled = true;
	bAutoActivate = false;
	bIsEditorOnly = true;
	SetHiddenInGame(true);
#if WITH_EDITORONLY_DATA
	SetIsVisualizationComponent(true);
#endif
}

#if WITH_EDITOR
void UDebugIconComponent::OnConstruction()
{
	if (!IconPath.Equals(CachedIcon.ToString()))
		CachedIcon = FSoftObjectPath(IconPath);
	SetSprite(CachedIcon.LoadSynchronous());
	
	TArray<USceneComponent*> Components;
	GetOwner()->GetComponents<USceneComponent>(Components);
	Components.Remove(GetOwner()->GetRootComponent());
	Components.Remove(this);
	
	SetVisibility(Components.Num() >= MaxComponents);
}

void UDebugIconComponent::SetupComponent(const AActor* Owner, const float Size)
{
	SetupAttachment(Owner->GetRootComponent());
	SetWorldScale3D(FVector{Size});
}

void UDebugIconComponent::OnChildAttached(USceneComponent* ChildComponent)
{
	ChildComponent->AttachToComponent(GetOwner()->GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform);
}
#endif

void UDebugIconComponent::BeginPlay()
{
	Super::BeginPlay();
	DestroyComponent();
}
