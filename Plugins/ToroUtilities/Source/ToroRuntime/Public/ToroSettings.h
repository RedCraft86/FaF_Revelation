// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InputMappingContext.h"
#include "DataTypes/MiscTypes.h"
#include "Engine/DeveloperSettings.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "ToroRuntime")
class TORORUNTIME_API UToroSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroSettings();

	SETTING_GETTER(UToroSettings)

	UPROPERTY(Config, EditAnywhere, Category = Settings, meta = (ReadOnlyKeys = true))
		TMap<EToroMapType, TSoftObjectPtr<UWorld>> MapRegistry;

	UPROPERTY(Config, EditAnywhere, Category = Settings)
		TSoftObjectPtr<UInputMappingContext> InputMappings;

	bool IsOnMap(const UObject* ContextObject, const EToroMapType MapType);

private:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
