// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

template <typename T>
struct TToroInterpolator
{
	T Current;
	T Target;
	float Speed;
	bool bConstant;
	
	FORCEINLINE virtual ~TToroInterpolator() = default;
	FORCEINLINE TToroInterpolator(): Speed(5.0f), bConstant(false) {}
	FORCEINLINE TToroInterpolator(const T InTarget, const float InSpeed = 5.0f)
		: Current(InTarget), Target(InTarget), Speed(InSpeed), bConstant(false)
	{}

	FORCEINLINE void SnapToTarget() { Current = Target; }
	FORCEINLINE virtual void Tick(const float InDeltaTime) {}
};

struct FToroInterpFloat final : TToroInterpolator<float>
{
	FORCEINLINE FToroInterpFloat() {}
	FORCEINLINE FToroInterpFloat(const float InTarget, const float InSpeed): TToroInterpolator(InTarget, InSpeed) {}
	FORCEINLINE bool IsComplete() const { return FMath::IsNearlyEqual(Current, Target); }
	FORCEINLINE virtual void Tick(const float InDeltaTime) override
	{
		if (!IsComplete()) Current = bConstant
			? FMath::FInterpConstantTo(Current, Target, InDeltaTime, Speed)
			: FMath::FInterpTo(Current, Target, InDeltaTime, Speed);
	}
};

struct FToroInterpVector2D final : TToroInterpolator<FVector2D>
{
	FORCEINLINE FToroInterpVector2D() {}
	FORCEINLINE FToroInterpVector2D(const FVector2D& InTarget, const float InSpeed): TToroInterpolator(InTarget, InSpeed) {}
	FORCEINLINE bool IsComplete() const { return Current.Equals(Target); }
	FORCEINLINE virtual void Tick(const float InDeltaTime) override
	{
		if (!IsComplete()) Current = bConstant
			? FMath::Vector2DInterpConstantTo(Current, Target, InDeltaTime, Speed)
			: FMath::Vector2DInterpTo(Current, Target, InDeltaTime, Speed);
	}
};

struct FToroInterpVector final : TToroInterpolator<FVector>
{
	FORCEINLINE FToroInterpVector() {}
	FORCEINLINE FToroInterpVector(const FVector& InTarget, const float InSpeed): TToroInterpolator(InTarget, InSpeed) {}
	FORCEINLINE bool IsComplete() const { return Current.Equals(Target); }
	FORCEINLINE virtual void Tick(const float InDeltaTime) override
	{
		if (!IsComplete()) Current = bConstant
			? FMath::VInterpConstantTo(Current, Target, InDeltaTime, Speed)
			: FMath::VInterpTo(Current, Target, InDeltaTime, Speed);
	}
};