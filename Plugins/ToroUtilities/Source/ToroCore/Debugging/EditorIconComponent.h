// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/BillboardComponent.h"
#include "EditorIconComponent.generated.h"

#define DECLARE_ACTOR_ICON TObjectPtr<UActorIconComponent> ActorEdIcon
#define UPDATE_ACTOR_ICON if (ActorEdIcon) { ActorEdIcon->OnConstruction(); }
#define DEFINE_ACTOR_ICON_SIZED(Size) \
	ActorEdIcon = CreateEditorOnlyDefaultSubobject<UEditorIconComponent>("ActorEdIcon"); \
	if (ActorEdIcon) { Var->SetupComponent(this, Size); }
#define DEFINE_ACTOR_ICON(Size) DEFINE_ACTOR_ICON_SIZED(0.5f)

UCLASS(MinimalAPI, NotBlueprintable, ClassGroup = (Debugging), meta = (BlueprintSpawnableComponent))
class UActorIconComponent final : public UBillboardComponent
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY() uint8 MaxComponents = 0;
	UPROPERTY() FString IconPath = TEXT("/Engine/EditorResources/EmptyActor.EmptyActor");
private:
	UPROPERTY() TSoftObjectPtr<UTexture2D> CachedIcon;
#endif
public:
	UActorIconComponent()
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
public:
	TOROCORE_API void OnConstruction()
	{
		if (const bool bNewIcon = !IconPath.Equals(CachedIcon.ToString()); !Sprite || bNewIcon)
		{
			if (bNewIcon) CachedIcon = FSoftObjectPath(IconPath);
			SetSprite(CachedIcon.LoadSynchronous());
		}
	
		TArray<USceneComponent*> Components;
		GetOwner()->GetComponents<USceneComponent>(Components);
		Components.Remove(GetOwner()->GetRootComponent());
		Components.Remove(this);
	
		SetVisibility(Components.Num() <= MaxComponents);
	}
	
	TOROCORE_API void SetupComponent(const AActor* Owner, const float Size)
	{
		SetupAttachment(Owner->GetRootComponent());
		SetWorldScale3D(FVector{Size});
	}
private:
	virtual bool CanAttachAsChild(const USceneComponent* ChildComponent, FName SocketName) const override { return false; }
	virtual void OnChildAttached(USceneComponent* ChildComponent) override
	{
		ChildComponent->AttachToComponent(GetOwner()->GetRootComponent(),
			FAttachmentTransformRules::KeepRelativeTransform);
	}
#endif
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		DestroyComponent();
	}
};