// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"
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

USTRUCT(BlueprintType)
struct FWidgetLoadInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Info)
		bool bIsOverlay = false;

	UPROPERTY(EditAnywhere, Category = Info)
		EGameMapType RequiredMap = EGameMapType::Gameplay;
};

UCLASS(Config = Game, DefaultConfig, DisplayName = "Game Settings")
class UGameSettings final : public UDeveloperSettings
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

	UPROPERTY(Config, EditAnywhere, Category = Game)
		TMap<TSoftClassPtr<UCommonUserWidget>, FWidgetLoadInfo> DefaultWidgets;

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = ContextObject))
		EGameMapType GetCurrentMapType(const UObject* ContextObject) const;

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = ContextObject))
		bool IsInMapType(const UObject* ContextObject, const EGameMapType MapType) const;
};
