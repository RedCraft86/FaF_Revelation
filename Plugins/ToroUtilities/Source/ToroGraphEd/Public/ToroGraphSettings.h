// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetters.h"
#include "ToroGraphSettings.generated.h"

UENUM(BlueprintType)
enum class EDataGraphLayoutStrategy : uint8
{
	Tree,
	ForceDirected,
};

UCLASS(Config = Editor, DefaultConfig, DisplayName = "Toro Graph Editor")
class TOROGRAPHED_API UToroGraphSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroGraphSettings() : OptimalDistance(100.f), AutoLayoutStrategy(EDataGraphLayoutStrategy::Tree)
		, MaxIteration(50), bFirstPassOnly(false), bRandomInit(false), InitTemperature(10.f), CoolDownRate(10.f)
	{
		CategoryName = TEXT("Editor");
		SectionName = TEXT("ToroGraphEd");
	}

	SETTING_GETTER(UToroGraphSettings)

	UPROPERTY(EditDefaultsOnly, Category = AutoArrange)
		float OptimalDistance;

	UPROPERTY(EditDefaultsOnly, Category = AutoArrange)
		EDataGraphLayoutStrategy AutoLayoutStrategy;

	UPROPERTY(EditDefaultsOnly, Category = AutoArrange)
		uint8 MaxIteration;

	UPROPERTY(EditDefaultsOnly, Category = AutoArrange)
		bool bFirstPassOnly;

	UPROPERTY(EditDefaultsOnly, Category = AutoArrange)
		bool bRandomInit;

	UPROPERTY(EditDefaultsOnly, Category = AutoArrange)
		float InitTemperature;

	UPROPERTY(EditDefaultsOnly, Category = AutoArrange)
		float CoolDownRate;
};