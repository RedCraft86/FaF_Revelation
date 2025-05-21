// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WireShapeData.generated.h"

USTRUCT(BlueprintType)
struct FWirePointData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WirePointData)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WirePointData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Size;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WirePointData)
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WirePointData)
        bool bRenderOnTopOfEverything;

    FWirePointData()
        : Location(FVector(0.0f))
        , Size(2.0f)
        , Color(FColor::Cyan)
        , bRenderOnTopOfEverything(false)
    {}
};

USTRUCT(BlueprintType)
struct FWireLineData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireLineData)
        FVector Start;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireLineData)
        FVector End;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireLineData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireLineData)
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireLineData)
        bool bRenderOnTopOfEverything;

    FWireLineData()
        : Start(FVector(0.0f))
        , End(FVector(0.0f))
        , Thickness(1.0f)
        , Color(FColor::Cyan)
        , bRenderOnTopOfEverything(false)
    {}
};

USTRUCT(BlueprintType)
struct FWireCircleData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCircleData)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCircleData)
        FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCircleData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCircleData, meta = (ClampMin = 1, UIMin = 1))
        uint8 NumOfSides;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCircleData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCircleData)
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCircleData)
        bool bRenderOnTopOfEverything;

    FWireCircleData()
        : Location(FVector(0.0f))
        , Rotation(FRotator(0.0f))
        , Radius(32.0f)
        , NumOfSides(16)
        , Thickness(1.0f)
        , Color(FColor::Cyan)
        , bRenderOnTopOfEverything(false)
    {}
};

USTRUCT(BlueprintType)
struct FWireArcData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireArcData)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireArcData)
        FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireArcData, meta = (ClampMin = -360.0f, UIMin = -360.0f, ClampMax = 360.0f, UIMax = 360.0f))
        float MinAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireArcData, meta = (ClampMin = -360.0f, UIMin = -360.0f, ClampMax = 360.0f, UIMax = 360.0f))
        float MaxAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireArcData, meta = (ClampMin = 1, UIMin = 1))
        float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireArcData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        uint8 Sections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireArcData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireArcData)
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireArcData)
        bool bRenderOnTopOfEverything;

    FWireArcData()
        : Location(FVector(0.0f))
        , Rotation(FRotator(0.0f))
        , MinAngle(0.0f)
        , MaxAngle(90.0f)
        , Radius(32.0f)
        , Sections(8)
        , Thickness(1.0f)
        , Color(FColor::Cyan)
        , bRenderOnTopOfEverything(false)
    {}
};

USTRUCT(BlueprintType)
struct FWireBoxData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireBoxData)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireBoxData)
        FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireBoxData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        FVector Extents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireBoxData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireBoxData)
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireBoxData)
        bool bRenderOnTopOfEverything;

    FWireBoxData()
        : Location(FVector(0.0f))
        , Rotation(FRotator(0.0f))
        , Extents(FVector(32.0f))
        , Thickness(1.0f)
        , Color(FColor::Cyan)
        , bRenderOnTopOfEverything(false)
    {}
};

USTRUCT(BlueprintType)
struct FWireSphereData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireSphereData)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireSphereData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireSphereData, meta = (ClampMin = 1, UIMin = 1))
        uint8 NumOfSides;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireSphereData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireSphereData)
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireSphereData)
        bool bRenderOnTopOfEverything;

    FWireSphereData()
        : Location(FVector(0.0f))
        , Radius(32.0f)
        , NumOfSides(16)
        , Thickness(1.0f)
        , Color(FColor::Cyan)
        , bRenderOnTopOfEverything(false)
    {}
};

USTRUCT(BlueprintType)
struct FWireCylinderData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCylinderData)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCylinderData)
        FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCylinderData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCylinderData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float HalfHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCylinderData, meta = (ClampMin = 1, UIMin = 1))
        uint8 NumOfSides;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCylinderData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCylinderData)
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCylinderData)
        bool bRenderOnTopOfEverything;

    FWireCylinderData()
        : Location(FVector(0.0f))
        , Rotation(FRotator(0.0f))
        , Radius(32.0f)
        , HalfHeight(64.0f)
        , NumOfSides(12)
        , Thickness(1.0f)
        , Color(FColor::Cyan)
        , bRenderOnTopOfEverything(false)
    {}
};

USTRUCT(BlueprintType)
struct FWireCapsuleData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCapsuleData)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCapsuleData)
        FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCapsuleData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCapsuleData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float HalfHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCapsuleData, meta = (ClampMin = 1, UIMin = 1))
        uint8 NumOfSides;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCapsuleData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCapsuleData)
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireCapsuleData)
        bool bRenderOnTopOfEverything;

    FWireCapsuleData()
        : Location(FVector(0.0f))
        , Rotation(FRotator(0.0f))
        , Radius(32.0f)
        , HalfHeight(64.0f)
        , NumOfSides(16)
        , Thickness(1.0f)
        , Color(FColor::Cyan)
        , bRenderOnTopOfEverything(false)
    {}
};

USTRUCT(BlueprintType)
struct FWireNavPathData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireNavPathData)
        TArray<FVector> Targets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireNavPathData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float LineThickness;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireNavPathData, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float PointSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireNavPathData)
        FLinearColor LineColor;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireNavPathData)
        FLinearColor PointColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireNavPathData)
        bool bRenderOnTopOfEverything;

    FWireNavPathData()
        : Targets({})
        , LineThickness(4.0f)
        , PointSize(16.0f)
        , LineColor(FColor::Yellow)
        , PointColor(FColor::Green)
        , bRenderOnTopOfEverything(true)
    {}
};

USTRUCT(BlueprintType)
struct FWireStringData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireStringData)
        FString String;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireStringData)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireStringData)
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireStringData, meta = (ClampMin = 1, UIMin = 1))
        uint8 FontScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireStringData)
        bool bDrawShadow;

    FWireStringData()
        : String(TEXT("Wire_String"))
        , Location(FVector(0.0f))
        , Color(FColor::Green)
        , FontScale(12)
        , bDrawShadow(true)
    {}
};