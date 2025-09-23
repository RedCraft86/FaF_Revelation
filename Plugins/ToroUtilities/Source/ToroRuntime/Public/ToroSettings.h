// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "InputMappingContext.h"
#include "DataTypes/MiscTypes.h"
#include "Engine/DeveloperSettings.h"
#include "SaveSystem/ToroSaveTypes.h"
#include "Helpers/ClassGetterMacros.h"
#include "MiscObjects/UDSSetterObject.h"
#include "UserInterface/ToroWidgetBase.h"
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

	UPROPERTY(Config, EditAnywhere, Category = Settings)
		TSoftObjectPtr<UMetaSoundSource> DefaultTheme;

	UPROPERTY(Config, EditAnywhere, Category = Settings)
		TSoftClassPtr<UUDSSetterObject> UDS_Setter;

	UPROPERTY(Config, EditAnywhere, Category = Settings)
		TMap<TSoftClassPtr<UToroSaveObject>, uint8> DefaultSaves;

	UPROPERTY(Config, EditAnywhere, Category = Settings)
		TArray<TSoftClassPtr<UToroWidgetBase>> DefaultWidgets;

	bool IsOnMap(const UObject* ContextObject, const EToroMapType MapType);

private:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
