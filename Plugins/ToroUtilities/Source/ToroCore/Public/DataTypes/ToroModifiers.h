// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroModifiers.generated.h"

template <typename T>
struct TOROCORE_API TToroModifier
{
	T Base;
	TMap<FName, T> Modifiers;

	TToroModifier() {}
	TToroModifier(const T InBase): Base(InBase) {}
	virtual ~TToroModifier() = default;

	virtual void Operation(T& Primary, const T Other) {}

	bool HasMod(const FName Key) const { return Modifiers.Contains(Key); }
	void AddMod(const FName Key, const T Value) { Modifiers.Add(Key, Value); }
	void RemoveMod(const FName Key) { Modifiers.Remove(Key); }
	T Evaluate() const
	{
		float Value = Base;
		for (const TPair<FName, float>& Pair : Modifiers)
			Operation(Value, Pair.Value);
		return Value;
	}
};

struct TOROCORE_API FToroMultiFloat final : TToroModifier<float>
{
	FToroMultiFloat(): TToroModifier(0.0f) {}
	FToroMultiFloat(const float InBase): TToroModifier(InBase) {}
	virtual void Operation(float& Primary, const float Other) override { Primary *= Other; }
};

struct TOROCORE_API FToroSumFloat final : TToroModifier<float>
{
	FToroSumFloat(): TToroModifier(0.0f) {}
	FToroSumFloat(const float InBase): TToroModifier(InBase) {}
	virtual void Operation(float& Primary, const float Other) override { Primary += Other; }
};

struct TOROCORE_API FToroMultiVector2D final : TToroModifier<FVector2D>
{
	FToroMultiVector2D(): TToroModifier(FVector2D::ZeroVector) {}
	FToroMultiVector2D(const FVector2D InBase): TToroModifier(InBase) {}
	virtual void Operation(FVector2D& Primary, const FVector2D Other) override { Primary *= Other; }
};

struct TOROCORE_API FToroSumVector2D final : TToroModifier<FVector2D>
{
	FToroSumVector2D(): TToroModifier(FVector2D::ZeroVector) {}
	FToroSumVector2D(const FVector2D InBase): TToroModifier(InBase) {}
	virtual void Operation(FVector2D& Primary, const FVector2D Other) override { Primary += Other; }
};