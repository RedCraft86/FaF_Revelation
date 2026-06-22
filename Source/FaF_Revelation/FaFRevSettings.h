// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UObject/SoftObjectPtr.h"
#include "DataAssets/ToroDatabase.h"
#include "Engine/DeveloperSettings.h"
#include "FaFRevSettings.generated.h"

UENUM(BlueprintType)
enum class EGameMapType : uint8
{
	MainMenu,
	Gameplay,
	Extras,
	MAX UMETA(Hidden)
};

UCLASS(Config = Game, DefaultConfig, DisplayName = "Game Settings")
class UFaFRevSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UFaFRevSettings();

	UFUNCTION(BlueprintPure, Category = Game, DisplayName = "Get Game Settings")
	[[nodiscard]] static const UFaFRevSettings* Get()
	{
		return GetDefault<UFaFRevSettings>();
	}

	UPROPERTY(Config, EditAnywhere, Category = Game)
		FName DemoName;

	UPROPERTY(Config, EditAnywhere, Category = Game, meta = (ArraySizeEnum = "/Script/FaF_Revelation.EGameMapType"))
		TSoftObjectPtr<UWorld> GameMaps[static_cast<uint8>(EGameMapType::MAX)];

	UPROPERTY(Config, EditAnywhere, Category = Game)
		TSet<TSoftObjectPtr<UToroDatabase>> Databases;

	UFUNCTION(BlueprintPure, Category = Game)
		FString GetVersionLabel() const;

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = ContextObject))
		bool IsInMapType(const UObject* ContextObject, const EGameMapType MapType) const;

	UFUNCTION(BlueprintPure, Category = Game, meta = (DeterminesOutputType = Class))
		UToroDatabase* GetDatabase(UPARAM(meta=(AllowAbstract=false)) const TSubclassOf<UToroDatabase> Class) const;

	template<typename T>
	T* GetDatabase()
	{
		static_assert(TIsDerivedFrom<T, UToroDatabase>::IsDerived, 
			TEXT("T must derive from UToroDatabase"));
		return Cast<T>(GetDatabase(T::StaticClass()));
	}
};
