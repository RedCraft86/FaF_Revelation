// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "NoticeWidget.h"
#include "MessageManager.h"
#include "Widgets/ExpressiveTextBlock.h"

UNoticeWidget::UNoticeWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UNoticeWidget::OnNoticeMessage(const FGenericMessage& InNotice)
{
	if (InNotice.IsValid())
	{
		NoticeText->SetText(InNotice.Content);
		PlayAnimationForward(FadeAnim);
	}
	else
	{
		PlayAnimationReverse(FadeAnim);
	}
}

void UNoticeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GetOrAddAnimationState(FadeAnim);
	if (UMessageManager* Manager = UMessageManager::Get(this))
	{
		Manager->OnNoticeText.BindUObject(this, &UNoticeWidget::OnNoticeMessage);
	}
}

void UNoticeWidget::NativeDestruct()
{
	UMessageManager* Manager = UMessageManager::Get(this);
	if (Manager && Manager->OnNoticeText.GetUObject() == this)
	{
		Manager->OnNoticeText.Unbind();
	}
	Super::NativeDestruct();
}
