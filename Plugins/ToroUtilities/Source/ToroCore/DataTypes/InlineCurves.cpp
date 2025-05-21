// Copyright (C) RedCraft86. All Rights Reserved.

#include "InlineCurves.h"

FRichCurve* FInlineFloatCurve::GetRichCurve()
{
	return Curve.GetRichCurve();
}

const FRichCurve* FInlineFloatCurve::GetRichCurveConst() const
{
	return Curve.GetRichCurveConst();
}

bool FInlineFloatCurve::HasAnyData() const
{
	return GetRichCurveConst()->HasAnyData();
}

UCurveFloat* FInlineFloatCurve::GetCurveAsset() const
{
	return Curve.ExternalCurve;
}

float FInlineFloatCurve::GetValue(float InTime) const
{
	return GetRichCurveConst()->Eval(InTime);
}

void FInlineFloatCurve::GetTimeRange(float& Min, float& Max) const
{
	GetRichCurveConst()->GetTimeRange(Min, Max);
}

void FInlineFloatCurve::GetTimeRange(double& Min, double& Max) const
{
	float X, Y;
	GetTimeRange(X, Y);
	Min = X; Max = Y;
}

void FInlineFloatCurve::GetValueRange(float& Min, float& Max) const
{
	GetRichCurveConst()->GetValueRange(Min, Max);
}

void FInlineFloatCurve::GetValueRange(double& Min, double& Max) const
{
	float X, Y;
	GetValueRange(X, Y);
	Min = X; Max = Y;
}

FRichCurve* FInlineVectorCurve::GetRichCurve(int32 Index)
{
	return Curve.GetRichCurve(Index);
}

const FRichCurve* FInlineVectorCurve::GetRichCurveConst(int32 Index) const
{
	return Curve.GetRichCurveConst(Index);
}

bool FInlineVectorCurve::HasAnyData() const
{
	return GetRichCurveConst(0)->HasAnyData()
		|| GetRichCurveConst(1)->HasAnyData()
		|| GetRichCurveConst(2)->HasAnyData();
}

UCurveVector* FInlineVectorCurve::GetCurveAsset() const
{
	return Curve.ExternalCurve;
}

FVector FInlineVectorCurve::GetValue(float InTime) const
{
	return {
		GetRichCurveConst(0)->Eval(InTime),
		GetRichCurveConst(1)->Eval(InTime),
		GetRichCurveConst(2)->Eval(InTime)
	};
}

void FInlineVectorCurve::GetTimeRange(float& Min, float& Max) const
{
	float Min1, Min2, Min3;
	float Max1, Max2, Max3;

	GetRichCurveConst(0)->GetTimeRange(Min1, Max1);
	GetRichCurveConst(1)->GetTimeRange(Min2, Max2);
	GetRichCurveConst(2)->GetTimeRange(Min3, Max3);

	Min = FMath::Min3(Min1, Min2, Min3);
	Max = FMath::Max3(Max1, Max2, Max3);
}

void FInlineVectorCurve::GetTimeRange(double& Min, double& Max) const
{
	float X, Y;
	GetTimeRange(X, Y);
	Min = X; Max = Y;
}

void FInlineVectorCurve::GetValueRange(FVector& Min, FVector& Max) const
{
	float MinX, MinY, MinZ;
	float MaxX, MaxY, MaxZ;

	GetRichCurveConst(0)->GetValueRange(MinX, MaxX);
	GetRichCurveConst(1)->GetValueRange(MinY, MaxY);
	GetRichCurveConst(2)->GetValueRange(MinZ, MaxZ);

	Min = { MinX, MinY, MinZ };
	Max = { MaxX, MaxY, MaxZ };
}

FRichCurve* FInlineColorCurve::GetRichCurve(int32 Index)
{
	if (Index < 0 || Index >= 4)
	{
		return nullptr;
	}

	if (IsValid(Curve.ExternalCurve))
	{
		return &Curve.ExternalCurve->FloatCurves[Index];
	}
	
	return &Curve.ColorCurves[Index];
}

const FRichCurve* FInlineColorCurve::GetRichCurveConst(int32 Index) const
{
	if (Index < 0 || Index >= 4)
	{
		return nullptr;
	}

	if (IsValid(Curve.ExternalCurve))
	{
		return &Curve.ExternalCurve->FloatCurves[Index];
	}
	
	return &Curve.ColorCurves[Index];
}

bool FInlineColorCurve::HasAnyData() const
{
	return GetRichCurveConst(0)->HasAnyData()
		|| GetRichCurveConst(1)->HasAnyData()
		|| GetRichCurveConst(2)->HasAnyData()
		|| GetRichCurveConst(3)->HasAnyData();
}

UCurveLinearColor* FInlineColorCurve::GetCurveAsset() const
{
	return Curve.ExternalCurve;
}

FLinearColor FInlineColorCurve::GetValue(float InTime) const
{
	FLinearColor Result = {
		FMath::Max(0.0f, GetRichCurveConst(0)->Eval(InTime)),
		FMath::Max(0.0f, GetRichCurveConst(1)->Eval(InTime)),
		FMath::Max(0.0f, GetRichCurveConst(2)->Eval(InTime))
	};
	
	const FRichCurve* Alpha = GetRichCurveConst(3);
	Result.A = Alpha->GetNumKeys() == 0 ? 1.0f : FMath::Max(0.0f, Alpha->Eval(InTime));

	return Result;
}

void FInlineColorCurve::GetTimeRange(float& Min, float& Max) const
{
	float Min1, Min2, Min3, Min4;
	float Max1, Max2, Max3, Max4;

	GetRichCurveConst(0)->GetTimeRange(Min1, Max1);
	GetRichCurveConst(1)->GetTimeRange(Min2, Max2);
	GetRichCurveConst(2)->GetTimeRange(Min3, Max3);
	GetRichCurveConst(3)->GetTimeRange(Min4, Max4);

	Min = FMath::Min3(Min1, Min2, FMath::Min(Min3, Min4));
	Max = FMath::Max3(Max1, Max2, FMath::Min(Max3, Max4));
}

void FInlineColorCurve::GetTimeRange(double& Min, double& Max) const
{
	float X, Y;
	GetTimeRange(X, Y);
	Min = X; Max = Y;
}

void FInlineColorCurve::GetValueRange(FLinearColor& Min, FLinearColor& Max) const
{
	float MinR, MinG, MinB, MinA;
	float MaxR, MaxG, MaxB, MaxA;

	GetRichCurveConst(0)->GetValueRange(MinR, MaxR);
	GetRichCurveConst(1)->GetValueRange(MinG, MaxG);
	GetRichCurveConst(2)->GetValueRange(MinB, MaxB);

	const FRichCurve* Alpha = GetRichCurveConst(3);
	const bool HasAlpha = Alpha->GetNumKeys() > 0;
	Alpha->GetTimeRange(MinA, MaxA);

	Min = { MinR, MinG, MinB, HasAlpha ? MinA : 1.0f };
	Max = { MaxR, MaxG, MaxB, HasAlpha ? MaxA : 1.0f };
}

bool UInlineCurvesLibrary::HasAnyFloatCurveData(const FInlineFloatCurve& InCurve)
{
	return InCurve.HasAnyData();
}

UCurveFloat* UInlineCurvesLibrary::GetFloatCurveAsset(const FInlineFloatCurve& InCurve)
{
	return InCurve.GetCurveAsset();
}

float UInlineCurvesLibrary::GetFloatCurveValue(const FInlineFloatCurve& InCurve, float InTime)
{
	return InCurve.GetValue(InTime);
}

void UInlineCurvesLibrary::GetFloatCurveTimeRange(const FInlineFloatCurve& InCurve, float& TimeMin, float& TimeMax)
{
	InCurve.GetTimeRange(TimeMin, TimeMax);
}

void UInlineCurvesLibrary::GetFloatCurveValueRange(const FInlineFloatCurve& InCurve, float& ValueMin, float& ValueMax)
{
	InCurve.GetValueRange(ValueMin, ValueMax);
}

bool UInlineCurvesLibrary::HasAnyVectorCurveData(const FInlineVectorCurve& InCurve)
{
	return InCurve.HasAnyData();
}

UCurveVector* UInlineCurvesLibrary::GetVectorCurveAsset(const FInlineVectorCurve& InCurve)
{
	return InCurve.GetCurveAsset();
}

FVector UInlineCurvesLibrary::GetVectorCurveValue(const FInlineVectorCurve& InCurve, float InTime)
{
	return InCurve.GetValue(InTime);
}

void UInlineCurvesLibrary::GetVectorCurveTimeRange(const FInlineVectorCurve& InCurve, float& TimeMin, float& TimeMax)
{
	InCurve.GetTimeRange(TimeMin, TimeMax);
}

void UInlineCurvesLibrary::GetVectorCurveValueRange(const FInlineVectorCurve& InCurve, FVector& ValueMin, FVector& ValueMax)
{
	InCurve.GetValueRange(ValueMin, ValueMax);
}

bool UInlineCurvesLibrary::HasAnyColorCurveData(const FInlineColorCurve& InCurve)
{
	return InCurve.HasAnyData();
}

UCurveLinearColor* UInlineCurvesLibrary::GetColorCurveAsset(const FInlineColorCurve& InCurve)
{
	return InCurve.GetCurveAsset();
}

FLinearColor UInlineCurvesLibrary::GetColorCurveValue(const FInlineColorCurve& InCurve, float InTime)
{
	return InCurve.GetValue(InTime);
}

void UInlineCurvesLibrary::GetColorCurveTimeRange(const FInlineColorCurve& InCurve, float& TimeMin, float& TimeMax)
{
	InCurve.GetTimeRange(TimeMin, TimeMax);
}

void UInlineCurvesLibrary::GetColorCurveValueRange(const FInlineColorCurve& InCurve, FLinearColor& ValueMin, FLinearColor& ValueMax)
{
	InCurve.GetValueRange(ValueMin, ValueMax);
}
