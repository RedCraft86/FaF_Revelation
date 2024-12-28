// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroMeshGenBase.h"
#include "Components/SplineComponent.h"
#include "ToroSplineGenBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AToroSplineGenBase : public AToroMeshGenBase
{
	GENERATED_BODY()

public:
	
	AToroSplineGenBase() : SplineType(ESplinePointType::Curve), bClosedLoop(false)
	{
		SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
		SplineComponent->AddSplinePoint(FVector(50.0f, 50.0f, 0.0f), ESplineCoordinateSpace::Local);
		SetRootComponent(SplineComponent);

		SceneRoot->SetupAttachment(SplineComponent);
	}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<USplineComponent> SplineComponent;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bClosedLoop;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
		TEnumAsByte<ESplinePointType::Type> SplineType;

#if WITH_EDITOR
protected:
	virtual void OnConstruction(const FTransform& Transform) override
	{
		Super::OnConstruction(Transform);
		SplineComponent->SetClosedLoop(bClosedLoop);
		for (int i = 0; i < SplineComponent->GetNumberOfSplinePoints(); i++)
			SplineComponent->SetSplinePointType(i, SplineType);
	}
#endif
};
