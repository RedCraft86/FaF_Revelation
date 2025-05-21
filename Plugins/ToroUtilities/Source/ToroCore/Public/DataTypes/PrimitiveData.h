// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/SplineMeshComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PrimitiveData.generated.h"

USTRUCT(BlueprintType)
struct TOROCORE_API FStaticMeshData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshData, meta = (DisplayPriority = 0))
        TSoftObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshData, meta = (DisplayPriority = 0))
        TArray<TSoftObjectPtr<UMaterialInterface>> Materials;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshData, meta = (DisplayPriority = 0))
        bool bCastShadows;

    FStaticMeshData() : Mesh(nullptr), Materials({}), bCastShadows(true) {}
    FStaticMeshData(const TSoftObjectPtr<UStaticMesh>& InMesh, const TArray<TSoftObjectPtr<UMaterialInterface>>& InMaterials = {}, const bool bInCastShadows = true)
        : Mesh(InMesh)
        , Materials(InMaterials)
        , bCastShadows(bInCastShadows)
    {}
    
    friend FArchive& operator<<(FArchive& Ar, FStaticMeshData& Data)
    {
        Ar << Data.Mesh;
        Ar << Data.Materials;
        Ar << Data.bCastShadows;
        return Ar;
    }
    
    void FillMaterials();
    FORCEINLINE bool IsValidData() const { return !Mesh.IsNull(); }
    void FromStaticMeshComponent(const UStaticMeshComponent* InComponent);
    void ToStaticMeshComponent(UStaticMeshComponent* InComponent) const;
};

USTRUCT(BlueprintType)
struct TOROCORE_API FTransformMeshData : public FStaticMeshData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshData, meta = (DisplayPriority = 1))
        FTransform Transform;

    FTransformMeshData() : Transform(FTransform::Identity) {}
    FStaticMeshData operator*() const { return {Mesh, Materials, bCastShadows}; }
    
    friend FArchive& operator<<(FArchive& Ar, FTransformMeshData& Data)
    {
        Ar << Data.Mesh;
        Ar << Data.Materials;
        Ar << Data.bCastShadows;
        Ar << Data.Transform;
        return Ar;
    }
};

USTRUCT(BlueprintType)
struct TOROCORE_API FSplineMeshData : public FStaticMeshData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshData, meta = (DisplayPriority = -1))
        TEnumAsByte<ESplineMeshAxis::Type> MeshAxis;

    FSplineMeshData() : MeshAxis(ESplineMeshAxis::X) {}
    FStaticMeshData operator*() const { return {Mesh, Materials, bCastShadows}; }
    
    friend FArchive& operator<<(FArchive& Ar, FSplineMeshData& Data)
    {
        Ar << Data.Mesh;
        Ar << Data.Materials;
        Ar << Data.bCastShadows;
        Ar << Data.MeshAxis;
        return Ar;
    }
};

USTRUCT(BlueprintType)
struct TOROCORE_API FActorBoundsCheckParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
        TEnumAsByte<ECollisionChannel> LineTraceChannel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params, meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 1.0f, UIMax = 1.0f))
        FVector BoundingBoxLerp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
        bool bOnlyCollidingComponents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
        bool bIncludeFromChildActors;

    FActorBoundsCheckParams()
        : LineTraceChannel(ECC_Visibility)
        , BoundingBoxLerp(FVector(1.0f))
        , bOnlyCollidingComponents(true)
        , bIncludeFromChildActors(false)
    {}
	
    FActorBoundsCheckParams(const ECollisionChannel InLineTraceChannel, const FVector& InBoundingBoxLerp,
        const bool bInOnlyCollidingComponents, const bool bInIncludeFromChildActors)
        : LineTraceChannel(InLineTraceChannel)
        , BoundingBoxLerp(InBoundingBoxLerp)
        , bOnlyCollidingComponents(bInOnlyCollidingComponents)
        , bIncludeFromChildActors(bInIncludeFromChildActors)
    {}
	
    FActorBoundsCheckParams(const float InBoundingBoxLerp)
        : LineTraceChannel(ECC_Visibility)
        , BoundingBoxLerp(FVector(InBoundingBoxLerp))
        , bOnlyCollidingComponents(true)
        , bIncludeFromChildActors(false)
    {}
	
    TArray<FVector> ProcessVertices(const TArray<FVector>& InVertices, const FVector& Origin) const
    {
        TArray<FVector> Out {Origin};
        for (const FVector& Vector : InVertices)
        {
            Out.AddUnique(FVector(
                FMath::Lerp(Origin.X, Vector.X, BoundingBoxLerp.X),
                FMath::Lerp(Origin.Y, Vector.Y, BoundingBoxLerp.Y),
                FMath::Lerp(Origin.Z, Vector.Z, BoundingBoxLerp.Z)
            ));
        }
        return Out;
    }
};

USTRUCT(BlueprintType, meta = (DisableSplitPin, HasNativeMake = "/Script/ToroCore.PrimitiveDataLibrary.Make_PrimitiveCollision", HasNativeBreak = "/Script/ToroCore.PrimitiveDataLibrary.Break_PrimitiveCollision"))
struct TOROCORE_API FPrimitiveCollision
{
#if WITH_EDITOR
    friend class FPrimitiveCollisionCustomization;
#endif
    
    GENERATED_BODY()

    FPrimitiveCollision()
        : CollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics)
        , ObjectType(ECC_WorldStatic)
        , Responses({})
    {
        SetProfileName(UCollisionProfile::BlockAll_ProfileName);
    }
    
    FPrimitiveCollision(const FName& ProfileName)
        : CollisionProfileName(NAME_None)
    {
        SetProfileName(ProfileName);
    }
    
    FPrimitiveCollision(const TEnumAsByte<ECollisionEnabled::Type> InCollisionEnabled, const TEnumAsByte<ECollisionChannel> InObjectType, const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses = {})
        : CollisionEnabled(InCollisionEnabled)
        , ObjectType(InObjectType)
        , Responses(InResponses)
        , CollisionProfileName(UCollisionProfile::CustomCollisionProfileName)
    {
        FillResponsesMap();
    }

    friend FArchive& operator<<(FArchive& Ar, FPrimitiveCollision& Data)
    {
        Ar << Data.CollisionProfileName;
        Ar << Data.CollisionEnabled;
        Ar << Data.ObjectType;
        Ar << Data.Responses;
        return Ar;
    }

    void SetProfileName(const FName& InName);
    FName GetProfileName() const { return CollisionProfileName; }
    void SetCollisionEnabled(const ECollisionEnabled::Type InCollisionEnabled);
    ECollisionEnabled::Type GetCollisionEnabled() const { return CollisionEnabled; }
    void SetObjectType(const ECollisionChannel InObjectType);
    ECollisionChannel GetObjectType() const { return ObjectType; }
    void SetAllResponses(const ECollisionResponse InResponse);
    void SetResponse(const ECollisionChannel InChannel, const ECollisionResponse InResponse);
    ECollisionResponse GetResponse(const ECollisionChannel InChannel) const;

    void SetResponsesMap(const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses) { Responses = InResponses; }
    TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>> GetResponsesMap() const { return Responses; }

    void FillResponsesMap(const ECollisionResponse InResponses = ECR_Block);
    void FromPrimitiveComponent(const UPrimitiveComponent* InComponent);
    void ToPrimitiveComponent(UPrimitiveComponent* InComponent) const;
    
private:

    /* Type of collision enabled.
    * No Collision      : Will not create any representation in the physics engine. Cannot be used for spatial queries (raycasts, sweeps, overlaps) or simulation (rigid body, constraints). Best performance possible (especially for moving objects)
    * Query Only        : Only used for spatial queries (raycasts, sweeps, and overlaps). Cannot be used for simulation (rigid body, constraints). Useful for character movement and things that do not need physical simulation. Performance gains by keeping data out of simulation tree.
    * Physics Only      : Only used for physics simulation (rigid body, constraints). Cannot be used for spatial queries (raycasts, sweeps, overlaps). Useful for jiggly bits on characters that do not need per bone detection. Performance gains by keeping data out of query tree
    * Collision Enabled : Can be used for both spatial queries (raycasts, sweeps, overlaps) and simulation (rigid body, constraints).
    */
    UPROPERTY(EditAnywhere, Category = PrimitiveCollision) TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled;

    /* Enum indicating what type of object this should be considered as when it moves */
    UPROPERTY(EditAnywhere, Category = PrimitiveCollision) TEnumAsByte<ECollisionChannel> ObjectType;
    
    /* Channels and Responses */
    UPROPERTY() TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>> Responses;
    
    /* Collision Profile Name */
    UPROPERTY() FName CollisionProfileName;
};

UCLASS()
class TOROCORE_API UPrimitiveDataLibrary final : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshData")
        static void FillMaterials(UPARAM(ref) FStaticMeshData& Target);

    UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshData", DisplayName = "Is Valid")
        static bool IsValid_MeshData(const FStaticMeshData& Target);

    /* Gets the vertices/corner locations of the actor's bounding box.
    * @param Target - Actor to use.
    * @param bOnlyCollidingComponents - If enabled, only components that can collide will be used in calculation.
    * @param bIncludeFromChildActors - Whether child actors should be included in the calculation.
    * @param Origin - Origin of the actor.
    * @param BoxExtent - Bounding box extent of the actor.
    * @return Locations of the 8 (or less) vertices of the bounding box.
    */
    UFUNCTION(BlueprintPure, Category = Actor, meta = (DefaultToSelf = "Target"))
        static TArray<FVector> GetBoundingBoxVertices(const AActor* Target, const bool bOnlyCollidingComponents,
            const bool bIncludeFromChildActors, FVector& Origin, FVector& BoxExtent);

    UFUNCTION(BlueprintPure, Category = "Collision|PrimitiveCollision", meta = (BlueprintThreadSafe, AutoCreateRefTerm = "InResponses", AdvancedDisplay = "InCollisionEnabled, InObjectType, InResponses"))
        static FPrimitiveCollision Make_PrimitiveCollision(const FName InProfileName, const TEnumAsByte<ECollisionEnabled::Type> InCollisionEnabled,
            const TEnumAsByte<ECollisionChannel> InObjectType, const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses);

    UFUNCTION(BlueprintPure, Category = "Collision|PrimitiveCollision", meta = (BlueprintThreadSafe, AdvancedDisplay = "ProfileName, CollisionEnabled, ObjectType, Responses"))
        static void Break_PrimitiveCollision(const FPrimitiveCollision& Target, FName& ProfileName, TEnumAsByte<ECollisionEnabled::Type>& CollisionEnabled,
            TEnumAsByte<ECollisionChannel>& ObjectType, TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& Responses);

    UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
        static void SetCollisionProfileName(UPARAM(ref) FPrimitiveCollision& Target, const FName InProfileName);

    UFUNCTION(BlueprintPure, Category = "Collision|PrimitiveCollision", meta = (BlueprintAutocast = true))
        static FName GetCollisionProfileName(const FPrimitiveCollision& Target);

    UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
        static void SetAllCollisionResponses(UPARAM(ref) FPrimitiveCollision& Target, const ECollisionResponse InResponse);
    
    UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
        static void SetCollisionResponse(UPARAM(ref) FPrimitiveCollision& Target, const ECollisionChannel InChannel, const ECollisionResponse InResponse);

    UFUNCTION(BlueprintPure, Category = "Collision|PrimitiveCollision")
        static ECollisionResponse GetCollisionResponse(const FPrimitiveCollision& Target, const ECollisionChannel InChannel);

    UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
        static void SetCollisionEnabled(UPARAM(ref) FPrimitiveCollision& Target, const ECollisionEnabled::Type InEnabled);

    UFUNCTION(BlueprintPure, Category = "Collision|PrimitiveCollision")
        static ECollisionEnabled::Type GetCollisionEnabled(const FPrimitiveCollision& Target);

    UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
        static void SetCollisionObjectType(UPARAM(ref) FPrimitiveCollision& Target, const ECollisionChannel InObjectType);

    UFUNCTION(BlueprintPure, Category = "Collision|PrimitiveCollision")
        static ECollisionChannel GetCollisionObjectType(const FPrimitiveCollision& Target);

    UFUNCTION(BlueprintPure, Category = "Components|StaticMesh", meta = (DefaultToSelf = "Target"))
        static FStaticMeshData GetStaticMeshData(const UStaticMeshComponent* Target);

    UFUNCTION(BlueprintCallable, Category = "Components|StaticMesh", meta = (DefaultToSelf = "Target"))
        static void SetStaticMeshData(UStaticMeshComponent* Target, const FStaticMeshData& Data);

    UFUNCTION(BlueprintPure, Category = Collision, meta = (DefaultToSelf = "Target"))
        static FPrimitiveCollision GetPrimitiveCollision(const UPrimitiveComponent* Target);

    UFUNCTION(BlueprintCallable, Category = Collision, meta = (DefaultToSelf = "Target"))
        static void SetPrimitiveCollision(UPrimitiveComponent* Target, const FPrimitiveCollision& Data);
};