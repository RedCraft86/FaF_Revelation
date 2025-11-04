// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "VisionConeComponent.h"
#include "ComponentVisualizer.h"

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
			DrawWireSphereCappedCone(PDI, ConeComponent->GetComponentTransform(), ConeComponent->Settings.Distance,
				ConeComponent->Settings.ConeAngle, 16, 4, 16, FLinearColor::Red, SDPG_World);
		}
	}
};