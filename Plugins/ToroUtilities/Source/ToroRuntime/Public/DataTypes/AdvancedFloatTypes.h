// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AdvancedFloatTypes.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FToroFloatModifier
{
	GENERATED_BODY()
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = FloatModifier)
		FName Type = "None";
#endif
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatModifier)
		float Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatModifier)
		TMap<FName, float> Modifiers;

	FToroFloatModifier() : Base(0.0f) {}
	FToroFloatModifier(const float InBase) : Base(InBase) {}
	virtual ~FToroFloatModifier() = default;
	
	virtual float Evaluate() const { return Base; }
};

USTRUCT(BlueprintType, DisplayName = "Float Multiplier")
struct TORORUNTIME_API FToroFloatMulti : public FToroFloatModifier
{
	GENERATED_BODY()

	FToroFloatMulti()
	{
#if WITH_EDITORONLY_DATA
		Type = "Multiplication";
#endif
	}
	
	FToroFloatMulti(const float InBase) : Super(InBase)
	{
#if WITH_EDITORONLY_DATA
		Type = "Multiplication";
#endif
	}
	
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

USTRUCT(BlueprintType, DisplayName = "Float Sum")
struct TORORUNTIME_API FToroFloatSum : public FToroFloatModifier
{
	GENERATED_BODY()

	FToroFloatSum()
	{
#if WITH_EDITORONLY_DATA
		Type = "Addition";
#endif
	}
	
	FToroFloatSum(const float InBase) : Super(InBase)
	{
#if WITH_EDITORONLY_DATA
		Type = "Addition";
#endif
	}
	
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FloatInterp)
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

	UFUNCTION(BlueprintPure, Category = AdvancedFloat, DisplayName = "Evaluate (Multi)", meta = (CompactNodeTitle = "->"))
		static float EvaluateModifier_Multi(const FToroFloatMulti& InFloat) { return InFloat.Evaluate(); }

	UFUNCTION(BlueprintPure, Category = AdvancedFloat, DisplayName = "Evaluate (Sum)", meta = (CompactNodeTitle = "->"))
		static float EvaluateModifier_Sum(const FToroFloatSum& InFloat) { return InFloat.Evaluate(); }

	UFUNCTION(BlueprintPure, Category = AdvancedFloat, DisplayName = "Is Complete", meta = (CompactNodeTitle = "->"))
		static float IsComplete_Interp(const FToroFloatInterp& InFloat) { return InFloat.IsComplete(); }
	
	UFUNCTION(BlueprintCallable, Category = AdvancedFloat, DisplayName = "Snap to Target")
		static void SnapToTarget_Interp(UPARAM(ref) FToroFloatInterp& InFloat) { InFloat.SnapToTarget(); }
	
	UFUNCTION(BlueprintCallable, Category = AdvancedFloat, DisplayName = "Tick Interpolation")
		static void Tick_Interp(UPARAM(ref) FToroFloatInterp& InFloat, const float DeltaTime) { InFloat.Tick(DeltaTime); }
};