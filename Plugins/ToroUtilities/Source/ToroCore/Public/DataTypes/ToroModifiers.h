// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

template <typename T>
struct TToroModifier
{
	T Base;
	TMap<FName, T> Modifiers;

	FORCEINLINE TToroModifier() {}
	FORCEINLINE TToroModifier(const T InBase): Base(InBase) {}
	FORCEINLINE virtual ~TToroModifier() = default;

	FORCEINLINE bool HasMod(const FName Key) const { return Modifiers.Contains(Key); }
	FORCEINLINE void AddMod(const FName Key, const T Value) { Modifiers.Add(Key, Value); }
	FORCEINLINE void RemoveMod(const FName Key) { Modifiers.Remove(Key); }
	FORCEINLINE T Evaluate() const
	{
		T Value = Base;
		for (const TPair<FName, T>& Pair : Modifiers)
			Operation(Value, Pair.Value);
		return Value;
	}
protected:
	virtual void Operation(T& Primary, const T Other) const {}
};

struct FToroMultiFloat final : TToroModifier<float>
{
	FORCEINLINE FToroMultiFloat(): TToroModifier(0.0f) {}
	FORCEINLINE FToroMultiFloat(const float InBase): TToroModifier(InBase) {}
protected:
	virtual void Operation(float& Primary, const float Other) const override { Primary *= Other; }
};

struct FToroSumFloat final : TToroModifier<float>
{
	FORCEINLINE FToroSumFloat(): TToroModifier(0.0f) {}
	FORCEINLINE FToroSumFloat(const float InBase): TToroModifier(InBase) {}
protected:
	virtual void Operation(float& Primary, const float Other) const override { Primary += Other; }
};

struct FToroMultiVector2D final : TToroModifier<FVector2D>
{
	FORCEINLINE FToroMultiVector2D(): TToroModifier(FVector2D::ZeroVector) {}
	FORCEINLINE FToroMultiVector2D(const FVector2D InBase): TToroModifier(InBase) {}
protected:
	virtual void Operation(FVector2D& Primary, const FVector2D Other) const override { Primary *= Other; }
};

struct FToroSumVector2D final : TToroModifier<FVector2D>
{
	FORCEINLINE FToroSumVector2D(): TToroModifier(FVector2D::ZeroVector) {}
	FORCEINLINE FToroSumVector2D(const FVector2D InBase): TToroModifier(InBase) {}
protected:
	virtual void Operation(FVector2D& Primary, const FVector2D Other) const override { Primary += Other; }
};