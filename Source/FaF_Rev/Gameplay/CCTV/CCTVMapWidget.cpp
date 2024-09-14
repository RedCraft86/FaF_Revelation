// Copyright (C) RedCraft86. All Rights Reserved.

#include "CCTVMapWidget.h"
#include "CCTVMonitor.h"
#include "Animation/UMGSequencePlayer.h"

void UCCTVMapWidget::SetSelectedCamera(const FName InID)
{
	if (OwningMonitor) OwningMonitor->ChangeCamera(InID);
}

void UCCTVMapWidget::RemoveWidget()
{
	if (UUMGSequencePlayer* Player = PlayAnimation(FadeAnim, 0, 1, EUMGSequencePlayMode::Reverse))
	{
		Player->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer& PlayerRef)
		{
			RemoveFromParent();
		});
	}
}

void UCCTVMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(FadeAnim);
}
