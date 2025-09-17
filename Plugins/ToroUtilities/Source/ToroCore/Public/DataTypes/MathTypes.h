// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveLinearColor.h"
#include "MathTypes.generated.h"

UENUM(BlueprintType)
enum class EVectorDirection : uint8
{
	Forward,
	Right,
	Up
};

USTRUCT(BlueprintType)
struct TOROCORE_API FInlineFloatCurve final
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = InlineFloatCurve)
		FRuntimeFloatCurve Curve;

	FRichCurve* GetRichCurve();
	const FRichCurve* GetRichCurveConst() const;
	
	bool HasAnyData() const;
	UCurveFloat* GetCurveAsset() const;
	
	float GetValue(float InTime) const;
	void GetTimeRange(float& Min, float& Max) const;
	void GetTimeRange(double& Min, double& Max) const;
	void GetValueRange(float& Min, float& Max) const;
	void GetValueRange(double& Min, double& Max) const;
};

USTRUCT(BlueprintType)
struct TOROCORE_API FInlineVectorCurve final
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = InlineVectorCurve)
		FRuntimeVectorCurve Curve;

	FRichCurve* GetRichCurve(int32 Index);
	const FRichCurve* GetRichCurveConst(int32 Index) const;
	
	bool HasAnyData() const;
	UCurveVector* GetCurveAsset() const;
	
	FVector GetValue(float InTime) const;
	void GetTimeRange(float& Min, float& Max) const;
	void GetTimeRange(double& Min, double& Max) const;
	void GetValueRange(FVector& Min, FVector& Max) const;
};

USTRUCT(BlueprintType)
struct TOROCORE_API FInlineColorCurve final
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = InlineColorCurve)
		FRuntimeCurveLinearColor Curve;

	FRichCurve* GetRichCurve(int32 Index);
	const FRichCurve* GetRichCurveConst(int32 Index) const;
	
	bool HasAnyData() const;
	UCurveLinearColor* GetCurveAsset() const;
	
	FLinearColor GetValue(float InTime) const;
	void GetTimeRange(float& Min, float& Max) const;
	void GetTimeRange(double& Min, double& Max) const;
	void GetValueRange(FLinearColor& Min, FLinearColor& Max) const;
};
