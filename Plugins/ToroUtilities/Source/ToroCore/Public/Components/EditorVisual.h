// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/BillboardComponent.h"
#include "EditorVisual.generated.h"

#define DEFINE_VISUAL \
	if (!FApp::IsGame()) EdVisualComp = CreateEditorOnlyDefaultSubobject<UEditorVisualComponent>("EdVisual"); \
	if (EdVisualComp) EdVisualComp->SetupAttachment(GetRootComponent());

#define UPDATE_VISUAL_MAX_COMP(Max) if (EdVisualComp) EdVisualComp->SetMaxComponents(Max);
#define UPDATE_VISUAL_SIZE(Size) if (EdVisualComp) EdVisualComp->UpdateSize(Size);
#define UPDATE_VISUAL_ICON(Icon) if (EdVisualComp) EdVisualComp->UpdateIcon(Icon);

UCLASS(MinimalAPI, NotBlueprintable, NotBlueprintType)
class UEditorVisualComponent final : public UBillboardComponent
{
	GENERATED_BODY()

public:

	TOROCORE_API UEditorVisualComponent()
	{
		PrimaryComponentTick.bCanEverTick = true;
		bIsScreenSizeScaled = true;
		bAutoActivate = false;
		bIsEditorOnly = true;
		bTickInEditor = true;
		SetHiddenInGame(true);
#if WITH_EDITORONLY_DATA
		SetIsVisualizationComponent(true);
#endif
		UpdateSize();
		UpdateIcon();
	}

#if WITH_EDITOR
	TOROCORE_API void SetMaxComponents(const uint8 Max = 1) { MaxComps = Max; }
	TOROCORE_API void UpdateSize(const float Scale = 0.5f) { SetWorldScale3D(FVector(Scale)); }
	TOROCORE_API void UpdateIcon(const FString& TexturePath = TEXT("/Engine/EditorResources/EmptyActor.EmptyActor"))
	{
		SetSprite(LoadObject<UTexture2D>(nullptr, *TexturePath));
	}
#endif

private:

	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		if (!FApp::IsGame()) return;
		GetWorld()->GetTimerManager().SetTimerForNextTick([WeakThis = TWeakObjectPtr(this)]()
		{
			if (WeakThis.IsValid()) WeakThis->DestroyComponent();
		});
	}
#if WITH_EDITOR
	uint8 MaxComps = 1;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override
	{
		Super::TickComponent(DeltaTime, TickType, TickFunc);
		bool bShouldShow = true;
		if (MaxComps == 0)
		{
			bShouldShow = false;
		}
		else
		{
			uint8 SceneComps = 0;
			for (const UActorComponent* Comp : GetOwner()->GetComponents())
			{
				if (Comp && Comp != this && Comp->IsA(USceneComponent::StaticClass()))
				{
					SceneComps++;
					if (SceneComps >= MaxComps)
					{
						bShouldShow = false;
					}
				}
			}
		}
		SetVisibility(bShouldShow);
	}
#endif
};
