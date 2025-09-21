// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/GameUserSettings.h"
#include "ToroUserSettings.generated.h"

UCLASS()
class TORORUNTIME_API UToroUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

	public:

	UToroUserSettings() {}

	static UToroUserSettings* Get()
	{
		return GEngine ? Cast<UToroUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
	}
};
