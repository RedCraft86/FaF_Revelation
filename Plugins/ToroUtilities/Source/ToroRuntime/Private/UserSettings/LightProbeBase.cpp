// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/LightProbeBase.h"
#include "UserSettings/ToroUserSettings.h"

ALightProbeBase::ALightProbeBase() : bDisableProbe(false), bAffectedByLumenGI(true), bUsingLumen(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

bool ALightProbeBase::ShouldDisable() const
{
	return bDisableProbe || (bAffectedByLumenGI && bUsingLumen);
}

void ALightProbeBase::BeginPlay()
{
	Super::BeginPlay();
	if (UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		Settings->OnDynamicSettingsChanged.AddUObject(this, &ThisClass::OnSettingChanged);
		OnSettingChanged(Settings);
	}
}

void ALightProbeBase::OnSettingChanged(const UToroUserSettings* InSettings)
{
	bUsingLumen = InSettings->GetLumenGI() > 0;
}
