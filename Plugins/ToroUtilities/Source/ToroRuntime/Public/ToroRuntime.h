// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "InputMappingContext.h"
#include "Modules/ModuleManager.h"
#include "DataTypes/MiscDataTypes.h"
#include "Helpers/ClassGetterMacros.h"
#include "Engine/DeveloperSettings.h"
#include "MiscObjects/UDSSetterObject.h"
#include "UserInterface/ToroWidgetBase.h"
#include "ToroRuntime.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogToroRuntime, All, All);

class FToroRuntimeModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};

UCLASS(Config = Game, DefaultConfig, DisplayName = "ToroRuntime")
class TORORUNTIME_API UToroSettings final : public UDeveloperSettings
{
    GENERATED_BODY()

public:

    UToroSettings();

    SETTING_GETTER(UToroSettings)

    UPROPERTY(Config, EditAnywhere, Category = World, meta = (ReadOnlyKeys = true))
        TMap<EToroMapType, TSoftObjectPtr<UWorld>> MapRegistry;

    UPROPERTY(Config, EditAnywhere, Category = World)
        TSoftClassPtr<UUDSSetterObject> UDS_Setter;

    UPROPERTY(Config, EditAnywhere, Category = Defaults)
        TSoftObjectPtr<UMetaSoundSource> DefaultTheme;

    UPROPERTY(Config, EditAnywhere, Category = Defaults)
        TArray<TSoftClassPtr<UToroWidgetBase>> DefaultWidgets;

    UPROPERTY(Config, EditAnywhere, Category = LightProbes)
        TSoftObjectPtr<UMaterialInterface> LightProbePPM;

    UPROPERTY(Config, EditAnywhere, Category = LightProbes, meta = (ClampMin = 16, UIMin = 16, ClampMax = 32, UIMax = 32))
        uint8 MaxProbes;

    UPROPERTY(Config, EditAnywhere, Category = Settings)
        TSoftObjectPtr<UInputMappingContext> InputMappings;

    UPROPERTY(Config, EditAnywhere, Category = Settings)
        TSoftObjectPtr<UMaterialInterface> BrightnessPPM;

    UPROPERTY(Config, EditAnywhere, Category = Saves)
        FName DemoName;

    UPROPERTY(Config, EditAnywhere, Category = Saves)
        TMap<TSoftClassPtr<class UToroSaveObject>, uint8> InitSaves;

    bool IsOnMap(const UObject* ContextObject, const EToroMapType MapType);

private:
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
