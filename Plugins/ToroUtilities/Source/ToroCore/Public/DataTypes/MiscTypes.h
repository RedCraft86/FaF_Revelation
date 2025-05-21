// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MiscTypes.generated.h"

UENUM(BlueprintInternalUseOnly)
enum class EToroFoundPins : uint8
{
	Found,
	NotFound
};

UENUM(BlueprintInternalUseOnly)
enum class EToroValidPins : uint8
{
	Valid,
	NotValid
};

USTRUCT(BlueprintType, DisplayName = "Texture Data")
struct FToroTextureData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TextureData)
		TArray<FColor> Pixels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TextureData)
		FIntPoint Size;
	
	FToroTextureData() : Pixels({}), Size({0}) {}
	FToroTextureData(const TArray<FColor>& InPixels, const int32 SizeX, const int32 SizeY)
		: Pixels(InPixels), Size({SizeX, SizeY}) {}
	
	friend FArchive& operator<<(FArchive& Ar, FToroTextureData& TextureData)
	{
		Ar << TextureData.Pixels;
		Ar << TextureData.Size;
		return Ar;
	}
	
	bool HasValidData() const { return !Pixels.IsEmpty() && Size.Size() > 0 && Pixels.Num() == Size.X * Size.Y; }
};

USTRUCT(BlueprintType)
struct TOROCORE_API FGameCurrency
{
	GENERATED_BODY()

	FGameCurrency() : CurrencyUnits(0) {}
	FGameCurrency(const int64 InMoney) : CurrencyUnits(InMoney) {}
	friend FArchive& operator<<(FArchive& Ar, FGameCurrency& Data)
	{
		Ar << Data.CurrencyUnits;
		return Ar;
	}

	int64 GetAmount() const { return CurrencyUnits; }
	void SetAmount(const int64 InMoney) { CurrencyUnits = FMath::Max(0, InMoney); }
	bool HasEnoughMoney(const int64 InRequirement) const { return CurrencyUnits > FMath::Max(0, InRequirement); }
	void RemoveMoney(const int64 InRemoval) { CurrencyUnits = FMath::Max(0, CurrencyUnits - FMath::Max(0, InRemoval)); }
	void AddMoney(const int64 InAddition) { CurrencyUnits += FMath::Max(0, InAddition); }

private:
	
	UPROPERTY(EditAnywhere, Category = GameCurrency, meta = (ClampMin = 0, UIMin = 0))
		int64 CurrencyUnits;
};

UCLASS()
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