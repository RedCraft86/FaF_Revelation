// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroInterpolator.generated.h"

template <typename T>
struct TOROCORE_API TToroInterpolator
{
	T Current;
	T Target;
	float Speed;
	bool bConstant;
	
	virtual ~TToroInterpolator() = default;
	TToroInterpolator(): Speed(0.0f), bConstant(false) {}
	TToroInterpolator(const T InTarget, const float InSpeed = 5.0f)
		: Current(InTarget), Speed(InSpeed), bConstant(false)
	{}

	void SnapToTarget() { Current = Target; }
	virtual void Tick(const float InDeltaTime) {}
};

struct TOROCORE_API FToroInterpFloat final : TToroInterpolator<float>
{
	FToroInterpFloat() {}
	FToroInterpFloat(const float InTarget, const float InSpeed = 5.0f): TToroInterpolator(InTarget, InSpeed) {}
	bool IsComplete() const { return FMath::IsNearlyEqual(Current, Target); }
	virtual void Tick(const float InDeltaTime) override
	{
		if (!IsComplete()) Current = bConstant
			? FMath::FInterpConstantTo(Current, Target, InDeltaTime, Speed)
			: FMath::FInterpTo(Current, Target, InDeltaTime, Speed);
	}
};

struct TOROCORE_API FToroInterpVector2D final : TToroInterpolator<FVector2D>
{
	FToroInterpVector2D() {}
	FToroInterpVector2D(const FVector2D& InTarget, const float InSpeed = 5.0f): TToroInterpolator(InTarget, InSpeed) {}
	bool IsComplete() const { return Current.Equals(Target); }
	virtual void Tick(const float InDeltaTime) override
	{
		if (!IsComplete()) Current = bConstant
			? FMath::Vector2DInterpConstantTo(Current, Target, InDeltaTime, Speed)
			: FMath::Vector2DInterpTo(Current, Target, InDeltaTime, Speed);
	}
};

struct TOROCORE_API FToroInterpVector final : TToroInterpolator<FVector>
{
	FToroInterpVector() {}
	FToroInterpVector(const FVector& InTarget, const float InSpeed = 5.0f): TToroInterpolator(InTarget, InSpeed) {}
	bool IsComplete() const { return Current.Equals(Target); }
	virtual void Tick(const float InDeltaTime) override
	{
		if (!IsComplete()) Current = bConstant
			? FMath::VInterpConstantTo(Current, Target, InDeltaTime, Speed)
			: FMath::VInterpTo(Current, Target, InDeltaTime, Speed);
	}
};
