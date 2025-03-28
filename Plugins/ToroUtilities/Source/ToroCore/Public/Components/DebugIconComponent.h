﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/BillboardComponent.h"
#include "DebugIconComponent.generated.h"

#define UPDATE_DEBUG_ICON(Var) if (Var) DebugIcon->OnConstruction();
#define CREATE_DEBUG_ICON(Var) \
	Var = CreateEditorOnlyDefaultSubobject<UDebugIconComponent>(#Var); \
	if (Var) Var->SetupComponent(this, 0.5f);
#define CREATE_DEBUG_ICON_WITH_SIZE(Var, Size) \
	Var = CreateEditorOnlyDefaultSubobject<UDebugIconComponent>(#Var); \
	if (Var) Var->SetupComponent(this, Size);

UCLASS(MinimalAPI, NotBlueprintable, ClassGroup = (Debugging), meta = (BlueprintSpawnableComponent))
class UDebugIconComponent final : public UBillboardComponent
{
	GENERATED_BODY()

public:
	
	UDebugIconComponent();
#if WITH_EDITORONLY_DATA
	UPROPERTY() uint8 MaxComponents = 0;
	UPROPERTY() FString IconPath = TEXT("/Engine/EditorResources/EmptyActor.EmptyActor");
private:
	UPROPERTY() TSoftObjectPtr<UTexture2D> CachedIcon;
#endif
#if WITH_EDITOR
public:
	TOROCORE_API void OnConstruction();
	TOROCORE_API void SetupComponent(const AActor* Owner, const float Size);
private:
	virtual void OnChildAttached(USceneComponent* ChildComponent) override;
	virtual bool CanAttachAsChild(const USceneComponent* ChildComponent, FName SocketName) const override
	{
		return false;
	}
#endif
	virtual void BeginPlay() override;
};
