// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/HUD.h"
#include "Helpers/ClassGetters.h"
#include "ToroWidgetManager.generated.h"

UCLASS()
class TOROUTILITIES_API AToroWidgetManager : public AHUD
{
	GENERATED_BODY()

public:

	AToroWidgetManager();

	template <typename T = AToroWidgetManager>
	static T* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		const APlayerController* PC = UGameplayStatics::GetPlayerController(ContextObject, PlayerIndex);
		return PC ? Cast<T>(PC->GetHUD()) : nullptr;
	}
};
