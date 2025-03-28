﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DebugShapeComponent.h"
#include "ComponentVisualizer.h"
#include "ComponentVisualizer/VisualizerHelpers.h"

class FDebugShapeVisualizer final : public FComponentVisualizer
{
public:
	
	static TSharedPtr<FComponentVisualizer> MakeInstance()
	{
		return MakeShareable(new FDebugShapeVisualizer);
	}
	
private:

	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override
	{
		if (const UDebugShapeComponent* VizComponent = Cast<const UDebugShapeComponent>(Component))
		{
			for (const TPair<FName, FDebugPointData>& Point : VizComponent->DebugPoints)
			{
				PDI->DrawPoint(VizComponent->TransformLocation(Point.Value.Location), Point.Value.Color,
					Point.Value.Size, Point.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World);
			}

			for (const TPair<FName, FDebugLineData>& Line : VizComponent->DebugLines)
			{
				PDI->DrawLine(VizComponent->TransformLocation(Line.Value.Start),
					VizComponent->TransformLocation(Line.Value.End), Line.Value.Color,
					Line.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Line.Value.Thickness);
			}

			for (const TPair<FName, FDebugCircleData>& Circle : VizComponent->DebugCircles)
			{
				DrawCircle(PDI, VizComponent->TransformLocation(Circle.Value.Location),
					VizComponent->AsForwardVector(Circle.Value.Rotation),
					VizComponent->AsRightVector(Circle.Value.Rotation),
					Circle.Value.Color, Circle.Value.Radius, Circle.Value.NumOfSides,
					Circle.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Circle.Value.Thickness);
			}

			for (const TPair<FName, FDebugArcData>& Arc : VizComponent->DebugArcs)
			{
				VisualizerHelpers::DrawArcWithThickness(PDI, VizComponent->TransformLocation(Arc.Value.Location),
					VizComponent->AsForwardVector(Arc.Value.Rotation),
					VizComponent->AsRightVector(Arc.Value.Rotation),
					Arc.Value.MinAngle, Arc.Value.MaxAngle, Arc.Value.Radius, Arc.Value.Sections, Arc.Value.Color,
					Arc.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Arc.Value.Thickness);
			}

			for (const TPair<FName, FDebugBoxData>& Box : VizComponent->DebugBoxes)
			{
				DrawOrientedWireBox(PDI, VizComponent->TransformLocation(Box.Value.Location),
					VizComponent->AsForwardVector(Box.Value.Rotation),
					VizComponent->AsRightVector(Box.Value.Rotation),
					VizComponent->AsUpVector(Box.Value.Rotation),
					Box.Value.Extents, Box.Value.Color,
					Box.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Box.Value.Thickness);
			}

			for (const TPair<FName, FDebugSphereData>& Sphere : VizComponent->DebugSpheres)
			{
				DrawWireSphere(PDI, VizComponent->TransformLocation(Sphere.Value.Location),
					Sphere.Value.Color, Sphere.Value.Radius, Sphere.Value.NumOfSides,
					Sphere.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Sphere.Value.Thickness);
			}

			for (const TPair<FName, FDebugCylinderData>& Cylinder : VizComponent->DebugCylinders)
			{
				DrawWireCylinder(PDI, VizComponent->TransformLocation(Cylinder.Value.Location),
					VizComponent->AsForwardVector(Cylinder.Value.Rotation),
					VizComponent->AsRightVector(Cylinder.Value.Rotation),
					VizComponent->AsUpVector(Cylinder.Value.Rotation),
					Cylinder.Value.Color, Cylinder.Value.Radius, Cylinder.Value.HalfHeight, Cylinder.Value.NumOfSides,
					Cylinder.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Cylinder.Value.Thickness);
			}

			for (const TPair<FName, FDebugCapsuleData>& Capsule : VizComponent->DebugCapsules)
			{
				DrawWireCapsule(PDI, VizComponent->TransformLocation(Capsule.Value.Location),
					VizComponent->AsForwardVector(Capsule.Value.Rotation),
					VizComponent->AsRightVector(Capsule.Value.Rotation),
					VizComponent->AsUpVector(Capsule.Value.Rotation),
					Capsule.Value.Color, Capsule.Value.Radius, Capsule.Value.HalfHeight, Capsule.Value.NumOfSides,
					Capsule.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Capsule.Value.Thickness);
			}

			const int32 NumNavPoints = VizComponent->NavPathPoints.Num();
			for (int i = 0; i < NumNavPoints; i++)
			{
				PDI->DrawPoint(VizComponent->NavPathPoints[i], VizComponent->DebugNavPath.PointColor,
					VizComponent->DebugNavPath.PointSize, VizComponent->DebugNavPath.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World);

				if (i < NumNavPoints - 1)
				{
					const FVector This = VizComponent->NavPathPoints[i], Next = VizComponent->NavPathPoints[i + 1];
					if (!This.Equals(Next))
					{
						PDI->DrawLine( This, Next, VizComponent->DebugNavPath.LineColor,
					   VizComponent->DebugNavPath.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World,
					   VizComponent->DebugNavPath.LineThickness);
					}
				}
			}
		}
	}

	virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override
	{
		if (const UDebugShapeComponent* VizComponent = Cast<const UDebugShapeComponent>(Component))
		{
			for (const TPair<FName, FDebugStringData>& StringData : VizComponent->DebugStrings)
			{
				VisualizerHelpers::DrawText(View, Canvas, VizComponent->TransformLocation(StringData.Value.Location),
					StringData.Value.String.IsEmpty() ? FText::FromName(StringData.Key) : FText::FromString(StringData.Value.String),
					StringData.Value.FontScale, StringData.Value.Color, StringData.Value.bDrawShadow);
			}
		}
	}
};