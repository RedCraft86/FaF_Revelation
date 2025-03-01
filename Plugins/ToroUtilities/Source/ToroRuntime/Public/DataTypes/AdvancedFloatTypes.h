// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AdvancedFloatTypes.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FToroFloatModifier
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatModifier)
		float Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatModifier)
		TMap<FName, float> Modifiers;

	FToroFloatModifier() : Base(0.0f) {}
	FToroFloatModifier(const float InBase) : Base(InBase) {}
	virtual ~FToroFloatModifier() = default;
	
	void ClampBase(const float Min, const float Max)
	{
		Base = FMath::Clamp(Base, Min, Max);
	}
	
	void ClampModifiers(const float Min, const float Max)
	{
		for (auto It = Modifiers.CreateIterator(); It; ++It)
		{
			It->Value = FMath::Clamp(It->Value, Min, Max);
		}
	}

	void AddMod(const FName Key, const float Value) { Modifiers.Add(Key, Value); }
	void RemoveMod(const FName Key) { Modifiers.Remove(Key); }
	virtual float Evaluate() const
	{
		return Base;
	}
};

USTRUCT(BlueprintType, DisplayName = "Float Multiplier", meta = (ModifierType = "Multipliers"))
struct TORORUNTIME_API FToroFloatMulti : public FToroFloatModifier
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
struct TORORUNTIME_API FToroFloatSum : public FToroFloatModifier
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
struct TORORUNTIME_API FToroFloatInterp
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

	FToroFloatInterp() : Current(0.0f), Target(0.0f), InterpSpeed(5.0f), bConstant(false) {}
	FToroFloatInterp(const float InCurrent, const float InSpeed = 5.0f, const bool bInConstant = false)
		: Current(InCurrent), Target(InCurrent), InterpSpeed(InSpeed), bConstant(bInConstant)
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

UCLASS(DisplayName = "Advanced Float Library")
class TORORUNTIME_API UAdvancedFloatLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = AdvancedFloat, DisplayName = "Evaluate (Multi)")
	static float Evaluate_Multi(const FToroFloatMulti& InFloat)
	{
		return InFloat.Evaluate();
	}

	UFUNCTION(BlueprintCallable, Category = AdvancedFloat, DisplayName = "Add Modifier (Multi)")
	static void AddMod_Multi(UPARAM(ref) FToroFloatMulti& InFloat, const FName InKey, const float InValue)
	{
		InFloat.AddMod(InKey, InValue);
	}

	UFUNCTION(BlueprintCallable, Category = AdvancedFloat, DisplayName = "Remove Modifier (Multi)")
	static void RemoveMod_Multi(UPARAM(ref) FToroFloatMulti& InFloat, const FName InKey)
	{
		InFloat.RemoveMod(InKey);
	}

	UFUNCTION(BlueprintPure, Category = AdvancedFloat, DisplayName = "Evaluate (Sum)")
	static float Evaluate_Sum(const FToroFloatSum& InFloat)
	{
		return InFloat.Evaluate();
	}

	UFUNCTION(BlueprintCallable, Category = AdvancedFloat, DisplayName = "Add Modifier (Sum)")
	static void AddMod_Sum(UPARAM(ref) FToroFloatSum& InFloat, const FName InKey, const float InValue)
	{
		InFloat.AddMod(InKey, InValue);
	}

	UFUNCTION(BlueprintCallable, Category = AdvancedFloat, DisplayName = "Remove Modifier (Sum)")
	static void RemoveMod_Sum(UPARAM(ref) FToroFloatSum& InFloat, const FName InKey)
	{
		InFloat.RemoveMod(InKey);
	}
	
	UFUNCTION(BlueprintPure, Category = AdvancedFloat, DisplayName = "Get Interp Value")
	static float GetValue_Interp(const FToroFloatInterp& InInterp)
	{
		return InInterp.Current;
	}
	
	UFUNCTION(BlueprintPure, Category = AdvancedFloat, DisplayName = "Is Complete")
	static bool IsComplete_Interp(const FToroFloatInterp& InInterp)
	{
		return InInterp.IsComplete();
	}
	
	UFUNCTION(BlueprintCallable, Category = AdvancedFloat, DisplayName = "Snap to Target")
	static void SnapToTarget_Interp(UPARAM(ref) FToroFloatInterp& InInterp)
	{
		InInterp.SnapToTarget();
	}
	
	UFUNCTION(BlueprintCallable, Category = AdvancedFloat, DisplayName = "Tick Interpolation")
	static void Tick_Interp(UPARAM(ref) FToroFloatInterp& InInterp, const float DeltaTime)
	{
		InInterp.Tick(DeltaTime);
	}
};