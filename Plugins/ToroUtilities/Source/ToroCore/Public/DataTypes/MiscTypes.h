// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MiscTypes.generated.h"

UENUM(BlueprintType)
enum class EWindowsDialogueType : uint8
{
	Ok						= 0	UMETA(DisplayName = "OK"),
	OkCancel				= 1	UMETA(DisplayName = "OK, Cancel"),
	AbortRetryIgnore		= 2	UMETA(DisplayName = "Abort, Retry, and Ignore"),
	YesNoCancel				= 3	UMETA(DisplayName = "Yes, No, Cancel"),
	YesNo					= 4	UMETA(DisplayName = "Yes, No"),
	RetryCancel				= 5	UMETA(DisplayName = "Retry, Cancel"),
	CancelTryAgainContinue	= 6	UMETA(DisplayName = "Cancel, Try Again, Continue")
};

UENUM(BlueprintType)
enum class EWindowsDialogueIcon : uint8
{
	None		= 0,
	Error		= 16,
	Warning		= 48,
	Information	= 64
};

UENUM(BlueprintType)
enum class EWindowsDialogueResult : uint8
{
	None		= 0,
	OK			= 1,
	Cancel		= 2,
	Abort		= 3,
	Retry		= 4,
	Ignore		= 5,
	Yes			= 6,
	No			= 7,
	TryAgain	= 10,
	Continue	= 11
};

UENUM(BlueprintType)
enum class EVirtualKeyTriggerType : uint8
{
	Click,
	DoubleClick,
	Press,
	Release
};

USTRUCT(BlueprintType)
struct TOROCORE_API FActorBoundsCheckParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
		TEnumAsByte<ECollisionChannel> LineTraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params, meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 1.0f, UIMax = 1.0f))
		FVector BoundingBoxLerp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
		bool bOnlyCollidingComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
		bool bIncludeFromChildActors;

	FActorBoundsCheckParams()
		: LineTraceChannel(ECC_Visibility)
		, BoundingBoxLerp(FVector(1.0f))
		, bOnlyCollidingComponents(true)
		, bIncludeFromChildActors(false)
	{}
	
	FActorBoundsCheckParams(const ECollisionChannel InLineTraceChannel, const FVector& InBoundingBoxLerp,
		const bool bInOnlyCollidingComponents, const bool bInIncludeFromChildActors)
		: LineTraceChannel(InLineTraceChannel)
		, BoundingBoxLerp(InBoundingBoxLerp)
		, bOnlyCollidingComponents(bInOnlyCollidingComponents)
		, bIncludeFromChildActors(bInIncludeFromChildActors)
	{}
	
	FActorBoundsCheckParams(const float InBoundingBoxLerp)
		: LineTraceChannel(ECC_Visibility)
		, BoundingBoxLerp(FVector(InBoundingBoxLerp))
		, bOnlyCollidingComponents(true)
		, bIncludeFromChildActors(false)
	{}
	
	TArray<FVector> ProcessVertices(const TArray<FVector>& InVertices, const FVector& Origin) const
	{
		TArray<FVector> TestVectors {Origin};
		for (const FVector& Vector : InVertices)
		{
			TestVectors.AddUnique(FVector(
				FMath::Lerp(Origin.X, Vector.X, BoundingBoxLerp.X),
				FMath::Lerp(Origin.Y, Vector.Y, BoundingBoxLerp.Y),
				FMath::Lerp(Origin.Z, Vector.Z, BoundingBoxLerp.Z)
			));
		}

		return TestVectors;
	}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FGameCurrency
{
	GENERATED_BODY()

	FGameCurrency() : CurrencyUnits(0) {}
	FGameCurrency(const int64 InCents) : CurrencyUnits(InCents) {}
	friend FArchive& operator<<(FArchive& Ar, FGameCurrency& Data)
	{
		Ar << Data.CurrencyUnits;
		return Ar;
	}

	int64 GetAmount() const { return CurrencyUnits; }
	void SetAmount(const int64 InCents) { CurrencyUnits = FMath::Max(0, InCents); }
	bool HasEnoughMoney(const int64 InRequirement) const { return CurrencyUnits > FMath::Max(0, InRequirement); }
	void RemoveMoney(const int64 InRemoval) { CurrencyUnits = FMath::Max(0, CurrencyUnits - FMath::Max(0, InRemoval)); }
	void AddMoney(const int64 InAddition) { CurrencyUnits += FMath::Max(0, InAddition); }

private:
	
	UPROPERTY(EditAnywhere, Category = GameCurrency, meta = (ClampMin = 0, UIMin = 0))
		int64 CurrencyUnits;
};

UCLASS(DisplayName = "Currency Function Library")
class TOROCORE_API UGameCurrencyLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = GameCurrency)
		static int64 GetMoney(const FGameCurrency& Target) { return Target.GetAmount(); }

	UFUNCTION(BlueprintCallable, Category = GameCurrency)
		static void SetMoney(UPARAM(ref) FGameCurrency& Target, const int64 InMoney) { return Target.SetAmount(InMoney); }

	UFUNCTION(BlueprintPure, Category = GameCurrency)
		static bool HasEnoughMoney(const FGameCurrency& Target, const int64 InRequirement) { return Target.HasEnoughMoney(InRequirement); }

	UFUNCTION(BlueprintCallable, Category = GameCurrency)
		static void RemoveMoney(UPARAM(ref) FGameCurrency& Target, const int64 InRemoval) { return Target.RemoveMoney(InRemoval); }

	UFUNCTION(BlueprintCallable, Category = GameCurrency)
		static void AddMoney(UPARAM(ref) FGameCurrency& Target, const int64 InAddition) { return Target.AddMoney(InAddition); }
};