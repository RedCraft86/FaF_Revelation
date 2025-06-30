// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "SaveSystem/ToroGlobalSave.h"
#include "Inventory/InventoryComponent.h"
#include "FaFRevSaves.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FAFREVGAME_API FGamePhaseData
{
	GENERATED_BODY()

	UPROPERTY() float PlayTime;
	UPROPERTY() FInventoryData Inventory;

	FGamePhaseData(): PlayTime(0.0f) {}
	friend FArchive& operator<<(FArchive& Ar, FGamePhaseData& Data)
	{
		Ar << Data.PlayTime;
		Ar << Data.Inventory;
		return Ar;
	}
};

UCLASS()
class FAFREVGAME_API UFaFRevGlobalSave final : public UToroGlobalSave
{
	GENERATED_BODY()

public:

	UFaFRevGlobalSave() {}

	UPROPERTY(BlueprintReadOnly, Category = GlobalSave)
		TSet<FGuid> PhaseNodes;

protected:

	virtual void SerializeData(FArchive& Ar) override
	{
		Super::SerializeData(Ar);
		Ar << PhaseNodes;
	}
};

UCLASS()
class FAFREVGAME_API UFaFRevGameSave final : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UFaFRevGameSave() {}

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TArray<uint8> Sequence;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TMap<FGuid, FGamePhaseData> PhaseData;

	void SetPhaseInventory(const FGuid& InPhase, const FInventoryData& InInventory)
	{
		PhaseData.FindOrAdd(InPhase).Inventory = InInventory;
	}

	FInventoryData GetPhaseInventory(const FGuid& InPhase) const
	{
		return PhaseData.FindRef(InPhase).Inventory;
	}

	void SetPhasePlayTime(const FGuid& InPhase, float& InTime)
	{
		FGamePhaseData& Data = PhaseData.FindOrAdd(InPhase);
		Data.PlayTime = FMath::Min(Data.PlayTime, InTime);
		InTime = 0.0f;
	}

	float GetPhasePlayTime(const FGuid& InPhase) const
	{
		return PhaseData.FindRef(InPhase).PlayTime;
	}

	float GetTotalPlayTime() const
	{
		float Sum = 0.0f;
		for (const TPair<FGuid, FGamePhaseData>& Pair : PhaseData)
		{
			Sum += Pair.Value.PlayTime;
		}
		return Sum;
	}

	virtual void DeleteData() override
	{
		Super::DeleteData();
		Sequence.Empty();
		PhaseData.Empty();
	}

protected:

	virtual void SerializeData(FArchive& Ar) override
	{
		Super::SerializeData(Ar);
		Ar << Sequence;
		Ar << PhaseData;
	}
};
