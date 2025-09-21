// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InputMappingContext.h"
#include "DataTypes/MiscTypes.h"
#include "Engine/DeveloperSettings.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroRuntimeSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "ToroRuntime")
class TORORUNTIME_API UToroRuntimeSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroRuntimeSettings();

	SETTING_GETTER(UToroRuntimeSettings)

	UPROPERTY(Config, EditAnywhere, Category = Settings, meta = (ReadOnlyKeys = true))
		TMap<EToroMapType, TSoftObjectPtr<UWorld>> MapRegistry;

	UPROPERTY(Config, EditAnywhere, Category = Settings)
		TSoftObjectPtr<UInputMappingContext> InputMappings;

	bool IsOnMapType(const UObject* ContextObject, const EToroMapType MapType);

private:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
