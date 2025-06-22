// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetters.h"
#include "DataGraphEdSettings.generated.h"

UENUM(BlueprintType)
enum class EDataGraphLayoutStrategy : uint8
{
	Tree,
	ForceDirected,
};

UCLASS(Config = Editor, DefaultConfig, DisplayName = "Data Graph Editor")
class DATAGRAPHED_API UDataGraphEdSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UDataGraphEdSettings() : OptimalDistance(100.f), AutoLayoutStrategy(EDataGraphLayoutStrategy::Tree)
		, MaxIteration(50), bFirstPassOnly(false), bRandomInit(false), InitTemperature(10.f), CoolDownRate(10.f)
	{
		CategoryName = TEXT("Editor");
		SectionName = TEXT("DataGraphEd");
	}

	SETTING_GETTER(UDataGraphEdSettings)

	UPROPERTY(Config, EditDefaultsOnly, Category = AutoArrange)
		float OptimalDistance;

	UPROPERTY(VisibleAnywhere, Category = AutoArrange)
		EDataGraphLayoutStrategy AutoLayoutStrategy;

	UPROPERTY(Config, EditDefaultsOnly, Category = AutoArrange)
		uint8 MaxIteration;

	UPROPERTY(Config, EditDefaultsOnly, Category = AutoArrange)
		bool bFirstPassOnly;

	UPROPERTY(Config, EditDefaultsOnly, Category = AutoArrange)
		bool bRandomInit;

	UPROPERTY(Config, EditDefaultsOnly, Category = AutoArrange)
		float InitTemperature;

	UPROPERTY(Config, EditDefaultsOnly, Category = AutoArrange)
		float CoolDownRate;
};
