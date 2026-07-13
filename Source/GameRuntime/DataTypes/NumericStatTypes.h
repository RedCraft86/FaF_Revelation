// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UObject/NameTypes.h"

template<typename T>
class FNumericModifierList
{
public:

	FNumericModifierList()
		: BaseValue(0.0f)
	{}

	FNumericModifierList(const T Base)
		: BaseValue(Base)
	{}

	void AddOrUpdate(const FName Key, const T Value)
	{
		InnerList.Add(Key, Value);
	}

	void Remove(const FName Key)
	{
		InnerList.Remove(Key);
	}

	bool Contains(const FName Key)
	{
		return InnerList.Contains(Key);
	}

	void Clear()
	{
		InnerList.Empty();
	}

	void SetBase(const T Value)
	{
		BaseValue = Value;
	}

	T GetSum() const
	{
		T Result = BaseValue;
		for (const TPair<FName, T>& Pair : InnerList)
		{
			Result += Pair.Value;
		}
		return Result;
	}

	T GetProduct() const
	{
		T Result = BaseValue;
		for (const TPair<FName, T>& Pair : InnerList)
		{
			Result *= Pair.Value;
		}
		return Result;
	}

private:

	T BaseValue;
	TMap<FName, T> InnerList;
};

template<typename T>
class FNumericValueInterp
{
public:

	FNumericValueInterp()
		: Current(0), Target(0), InterpSpeed(5.0f), bConstant(false)
	{}

	FNumericValueInterp(const T InValue, const float InSpeed, const bool bInConstant)
		: Current(InValue), Target(InValue), InterpSpeed(InSpeed), bConstant(bInConstant)
	{}

	const T& GetValue() const
	{
		return Current;
	}

	void SetTarget(const T Value)
	{
		Target = Value;
	}

	void SnapToTarget()
	{
		Current = Target;
	}

	bool IsComplete() const
	{
		if constexpr (TIsFloatingPoint<T>::Value)
		{
			return FMath::IsNearlyEqual(Current, Target);
		}
		else
		{
			return Current.Equals(Target);
		}
	}

	bool TickInterp(const float DeltaTime)
	{
		if (IsComplete())
		{
			return false;
		}

		if (bConstant)
		{
			InterpConstantTo(DeltaTime);
		}
		else
		{
			InterpTo(DeltaTime);
		}

		return true;
	}

private:

	T Current, Target;
	float InterpSpeed;
	bool bConstant;

	static float GetMagnitude(const T Value)
	{
		if constexpr (TIsFloatingPoint<T>::Value)
		{
			return FMath::Abs(Value);
		}
		else
		{
			return Value.Size();
		}
	}

	void InterpTo(float DeltaTime)
	{
		if (InterpSpeed <= 0.0f)
		{
			Current = Target;
			return;
		}

		const T Dist = Target - Current;
		const float Alpha = FMath::Clamp(1.0f - FMath::Exp(-InterpSpeed * DeltaTime), 0.0f, 1.0f);
		Current += Dist * Alpha;
	}

	void InterpConstantTo(float DeltaTime)
	{
		if (InterpSpeed <= 0.f)
		{
			Current = Target;
			return;
		}

		const T Dist = Target - Current;
		const float Mag = GetMagnitude(Dist);
		const float Step = InterpSpeed * DeltaTime;

		if (Mag <= Step || Mag < SMALL_NUMBER)
		{
			Current = Target;
			return;
		}

		Current += (Dist * (Step / Mag));
	}
};