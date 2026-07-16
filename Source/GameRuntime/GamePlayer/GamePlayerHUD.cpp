// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GamePlayerHUD.h"
#include "GameSettings.h"

AGamePlayerHUD::AGamePlayerHUD()
{
}

void AGamePlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	if (MasterWidget)
	{
		const UGameSettings* Settings = UGameSettings::Get();
		const EGameMapType MapType = Settings->GetCurrentMapType(this);
		for (const TPair<TSoftClassPtr<UCommonUserWidget>, FWidgetLoadInfo>& Widget : Settings->DefaultWidgets)
		{
			if (Widget.Value.RequiredMap != MapType || !Widget.Key.LoadSynchronous())
			{
				continue;
			}

			if (Widget.Value.bIsOverlay)
			{
				MasterWidget->PushClassToOverlay(Widget.Key.Get());
			}
			else if (Widget.Key.Get()->IsChildOf(UCommonActivatableWidget::StaticClass()))
			{
				MasterWidget->PushToStack(Widget.Key.Get());
			}
		}
	}
}
