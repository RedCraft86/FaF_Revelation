// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathTypes.generated.h"

UENUM(BlueprintType)
enum class EVectorDirection : uint8
{
	Forward,
	Right,
	Up
};

USTRUCT(BlueprintInternalUseOnly)
struct TOROCORE_API FToroFloatModifier
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatModifier)
		float Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatModifier)
		TMap<FName, float> Modifiers;

	FToroFloatModifier(): Base(0.0f) {}
	FToroFloatModifier(const float InBase): Base(InBase) {}
	virtual ~FToroFloatModifier() = default;

	void AddMod(const FName Key, const float Value) { Modifiers.Add(Key, Value); }
	void RemoveMod(const FName Key) { Modifiers.Remove(Key); }
	virtual float Evaluate() const
	{
		return Base;
	}
};

USTRUCT(BlueprintType, DisplayName = "Float Multiplier", meta = (ModifierType = "Multipliers"))
struct TOROCORE_API FToroFloatMulti : public FToroFloatModifier
{
	GENERATED_BODY()

	FToroFloatMulti() {}
	FToroFloatMulti(const float InBase) : Super(InBase) {}
	virtual float Evaluate() const override
	{
		float Value = Base;
		for (const TPair<FName, float>& Pair : Modifiers)
		{
			if (!Pair.Key.IsNone()) Value *= Pair.Value;
		}
		return Value;
	}
};

USTRUCT(BlueprintType, DisplayName = "Float Sum", meta = (ModifierType = "Additions"))
struct TOROCORE_API FToroFloatSum : public FToroFloatModifier
{
	GENERATED_BODY()

	FToroFloatSum() {}
	FToroFloatSum(const float InBase) : Super(InBase) {}
	virtual float Evaluate() const override
	{
		float Value = Base;
		for (const TPair<FName, float>& Pair : Modifiers)
		{
			if (!Pair.Key.IsNone()) Value += Pair.Value;
		}
		return Value;
	}
};

USTRUCT(BlueprintType, DisplayName = "Float Interpolation")
struct TOROCORE_API FToroFloatInterp
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FloatInterp)
		float Current;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatInterp)
		float Target;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatInterp)
		float InterpSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatInterp)
		bool bConstant;

	FToroFloatInterp(): Current(0.0f), Target(0.0f), InterpSpeed(5.0f), bConstant(false) {}
	FToroFloatInterp(const float InTarget, const float InSpeed = 5.0f, const bool bInConstant = false)
		: Current(InTarget), Target(InTarget), InterpSpeed(InSpeed), bConstant(bInConstant)
	{}

	void SnapToTarget() { Current = Target; }
	bool IsComplete() const { return FMath::IsNearlyEqual(Current, Target); }
	void Tick(const float InDeltaTime)
	{
		if (IsComplete()) return;
		Current = bConstant ? FMath::FInterpConstantTo(Current, Target, InDeltaTime, InterpSpeed)
			: FMath::FInterpTo(Current, Target, InDeltaTime, InterpSpeed);
	}
};

USTRUCT(BlueprintType, DisplayName = "Vector Interpolation")
struct TOROCORE_API FToroVectorInterp
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FloatInterp)
		FVector Current;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatInterp)
		FVector Target;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatInterp)
		float InterpSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatInterp)
		bool bConstant;

	FToroVectorInterp() : Current(0.0f), Target(0.0f), InterpSpeed(5.0f), bConstant(false) {}
	FToroVectorInterp(const FVector& InTarget, const float InSpeed = 5.0f, const bool bInConstant = false)
		: Current(InTarget), Target(InTarget), InterpSpeed(InSpeed), bConstant(bInConstant)
	{}

	void SnapToTarget() { Current = Target; }
	bool IsComplete() const { return Current.Equals(Target); }
	void Tick(const float InDeltaTime)
	{
		if (IsComplete()) return;
		Current = bConstant ? FMath::VInterpTo(Current, Target, InDeltaTime, InterpSpeed)
			: FMath::VInterpConstantTo(Current, Target, InDeltaTime, InterpSpeed);
	}
};