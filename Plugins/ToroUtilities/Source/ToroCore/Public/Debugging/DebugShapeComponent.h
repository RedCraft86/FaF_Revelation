﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "DataTypes/WireShapeData.h"
#include "Components/ActorComponent.h"
#include "DebugShapeComponent.generated.h"

UCLASS(MinimalAPI, NotBlueprintable, ClassGroup = (Wireging), meta = (BlueprintSpawnableComponent))
class UWireShapeComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UWireShapeComponent()
	{
	    PrimaryComponentTick.bCanEverTick = false;
	    bAutoActivate = false;
	    bIsEditorOnly = true;
#if WITH_EDITORONLY_DATA
	    SetIsVisualizationComponent(true);
#endif
	}

#if WITH_EDITORONLY_DATA
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visuals)
        TMap<FName, FWirePointData> WirePoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visuals)
        TMap<FName, FWireLineData> WireLines;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visuals)
        TMap<FName, FWireCircleData> WireCircles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visuals)
        TMap<FName, FWireArcData> WireArcs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visuals)
        TMap<FName, FWireBoxData> WireBoxes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visuals)
        TMap<FName, FWireSphereData> WireSpheres;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visuals)
        TMap<FName, FWireCylinderData> WireCylinders;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visuals)
        TMap<FName, FWireCapsuleData> WireCapsules;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visuals)
        FWireNavPathData WireNavPath;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visuals)
        TMap<FName, FWireStringData> WireStrings;

    UPROPERTY(Transient) TArray<FVector> NavPathPoints;
#endif
#if WITH_EDITOR
    TOROCORE_API FVector TransformLocation(const FVector& LocalLocation) const
    {
        return GetOwner()->GetActorTransform().TransformPositionNoScale(LocalLocation);
    }
    TOROCORE_API FRotator TransformRotation(const FRotator& LocalRotation) const
    {
        return GetOwner()->GetActorTransform().TransformRotation(LocalRotation.Quaternion()).Rotator();
    }
    TOROCORE_API FVector AsForwardVector(const FRotator& LocalRotation) const
    {
        return TransformRotation(LocalRotation).Vector();
    }
    TOROCORE_API FVector AsRightVector(const FRotator& LocalRotation) const
    {
        return FRotationMatrix(TransformRotation(LocalRotation)).GetScaledAxis(EAxis::Y);
    }
    TOROCORE_API FVector AsUpVector(const FRotator& LocalRotation) const
    {
        return FRotationMatrix(TransformRotation(LocalRotation)).GetScaledAxis(EAxis::Z);
    }
    TOROCORE_API void UpdateNavPoints()
    {
        NavPathPoints.Empty();
	
        const int32 NumPoints = WireNavPath.Targets.Num();
        if (NumPoints < 2) return;

        for (int i = 0; i < NumPoints - 1; i++)
        {
            if (const UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,
                TransformLocation(WireNavPath.Targets[i]),
                TransformLocation(WireNavPath.Targets[i + 1])))
            {
                NavPathPoints.Append(NavPath->PathPoints);
            }
        }
    }
#endif
private:
	virtual void BeginPlay() override
	{
	    Super::BeginPlay();
	    DestroyComponent();
	}
#if WITH_EDITOR
    virtual void PostInitProperties() override
    {
        Super::PostInitProperties();
        UpdateNavPoints();
    }
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
    {
        Super::PostEditChangeProperty(PropertyChangedEvent);
        if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, WireNavPath))
        {
            UpdateNavPoints();
        }
    }
#endif
};
