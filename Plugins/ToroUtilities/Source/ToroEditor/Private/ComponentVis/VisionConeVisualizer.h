// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ComponentVisualizer.h"
#include "ComponentVis/VisualizerHelpers.h"
#include "Components/VisionConeComponent.h"

class FVisionConeVisualizer final : public FComponentVisualizer
{
public:
	
	static TSharedPtr<FComponentVisualizer> MakeInstance()
	{
		return MakeShareable(new FVisionConeVisualizer);
	}
	
private:

	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override
	{
		if (const UVisionConeComponent* ConeComponent = Cast<const UVisionConeComponent>(Component))
		{
			if (FMath::IsNearlyZero(ConeComponent->GetOuterAngle()) || FMath::IsNearlyEqual(ConeComponent->GetInnerAngle(), 90.0f))
			{
				DrawWireSphereCappedCone(PDI, ConeComponent->GetComponentTransform(), ConeComponent->VisionCone.Distance,
					ConeComponent->GetInnerAngle(), 16, 4, 16, FLinearColor::Red, SDPG_World);	
			}
			else
			{
				TArray<FVector> Verts;
				DrawWireCone(PDI, Verts, ConeComponent->GetComponentTransform(), ConeComponent->VisionCone.Distance,
					ConeComponent->GetInnerAngle(), 16, FLinearColor::Red, SDPG_World);

				DrawWireSphereCappedCone(PDI, ConeComponent->GetComponentTransform(), ConeComponent->VisionCone.Distance,
					ConeComponent->GetOuterAngle(), 16, 4, 16, FLinearColor::Yellow, SDPG_World);	
			}
		}
	}

	virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override
	{
		if (const UVisionConeComponent* ConeComponent = Cast<const UVisionConeComponent>(Component))
		{
			VisualizerHelpers::DrawText(View, Canvas, ConeComponent->GetComponentLocation(),
				FText::Format(INVTEXT("Trace: {0}\nAutomatic: {1}"), 
				UEnum::GetDisplayValueAsText(ConeComponent->TraceChannel),
				FText::FromString(LexToString(ConeComponent->bDetectCharacters))),
				12, FLinearColor::Green, true);
		}
	}
};