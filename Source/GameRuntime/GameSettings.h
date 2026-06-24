// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UObject/SoftObjectPtr.h"
#include "Engine/DeveloperSettings.h"
#include "GameSettings.generated.h"

UENUM(BlueprintType)
enum class EGameMapType : uint8
{
	MainMenu,
	Gameplay,
	Extras,
	MAX UMETA(Hidden)
};

UCLASS(Config = Game, DefaultConfig, DisplayName = "Game Settings")
class GAMERUNTIME_API UGameSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UGameSettings();

	UFUNCTION(BlueprintPure, Category = Game, DisplayName = "Get Game Settings")
	[[nodiscard]] static const UGameSettings* Get()
	{
		return GetDefault<UGameSettings>();
	}

	UPROPERTY(Config, EditAnywhere, Category = Game, meta = (ArraySizeEnum = "/Script/GameRuntime.EGameMapType"))
		TSoftObjectPtr<UWorld> GameMaps[static_cast<uint8>(EGameMapType::MAX)];

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = ContextObject))
		bool IsInMapType(const UObject* ContextObject, const EGameMapType MapType) const;
};
