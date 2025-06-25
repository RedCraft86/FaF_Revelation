// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/MessagingWidget.h"
#include "Animation/UMGSequencePlayer.h"
#include "Framework/ToroWidgetManager.h"
#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "EnhancedCodeFlow.h"

void UInputPreviewEntry::InitData(const FToroInputPrompt& InPreview)
{
	NameText->SetText(InPreview.Name);
	for (const FKey& Key : InPreview.Keys)
	{
		if (FRichImageRow Icon = GetKeyIcon(Key.GetFName()); Icon.Brush.GetResourceObject())
		{
			if (UImage* Image = WidgetTree->ConstructWidget<UImage>())
			{
				Image->SetBrush(Icon.Brush);
				KeysBox->AddChild(Image);
			}
		}
	}
}

UMessagingWidget::UMessagingWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 99;
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UMessagingWidget::QueueNotice(const UObject* ContextObject, const FToroSimpleMsg& InNotice)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UMessagingWidget* Widget = Manager->FindWidget<UMessagingWidget>())
		{
			Widget->AddNotice(InNotice);
		}
	}
}

void UMessagingWidget::QueueTitle(const UObject* ContextObject, const FToroSimpleMsg& InTitle)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UMessagingWidget* Widget = Manager->FindWidget<UMessagingWidget>())
		{
			Widget->AddTitle(InTitle);
		}
	}
}

void UMessagingWidget::ShowInputPrompt(const UObject* ContextObject, const FName InKey, const FToroInputPrompt& InData)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UMessagingWidget* Widget = Manager->FindWidget<UMessagingWidget>())
		{
			Widget->AddInputRow(InKey, InData);
		}
	}
}

void UMessagingWidget::HideInputPrompt(const UObject* ContextObject, const FName InKey)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UMessagingWidget* Widget = Manager->FindWidget<UMessagingWidget>())
		{
			Widget->RemoveInputRow(InKey);
		}
	}
}

void UMessagingWidget::ClearInputPrompts(const UObject* ContextObject)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UMessagingWidget* Widget = Manager->FindWidget<UMessagingWidget>())
		{
			Widget->ClearInputRows();
		}
	}
}

void UMessagingWidget::NextNotice()
{
	if (!Notices.IsEmpty())
	{
		ShowNotice(Notices[0]);
		Notices.RemoveAt(0);
	}
	else
	{
		PlayAnimationReverse(NoticeAnim);
	}
}

void UMessagingWidget::ShowNotice(const FToroSimpleMsg& InData)
{
	NoticeText->SetText(InData.Message);
	PlayAnimation(NoticeAnim);

	GetWorld()->GetTimerManager().SetTimer(NoticeTimer, this,
		&UMessagingWidget::NextNotice, InData.CalcTime(), false);
}

void UMessagingWidget::AddNotice(const FToroSimpleMsg& InNotice, const bool bImmediate)
{
	if (!InNotice.IsValidData()) return;
	if (!Notices.IsEmpty() && Notices[Notices.Num() - 1].Message.EqualTo(InNotice.Message)) return;

	if (bImmediate)
	{
		GetWorld()->GetTimerManager().ClearTimer(NoticeTimer);
		ShowNotice(InNotice);
	}
	else
	{
		Notices.Add(InNotice);
		if (!GetWorld()->GetTimerManager().IsTimerActive(NoticeTimer))
		{
			NextNotice();
		}
	}
}

void UMessagingWidget::NextTitle()
{
	if (!Titles.IsEmpty())
	{
		ShowNotice(Titles[0]);
		Titles.RemoveAt(0);
	}
	else
	{
		PlayAnimationReverse(TitleAnim);
	}
}

void UMessagingWidget::ShowTitle(const FToroSimpleMsg& InData)
{
	TitleText->SetText(InData.Message);
	PlayAnimation(TitleAnim);

	GetWorld()->GetTimerManager().SetTimer(TitleTimer, this,
		&UMessagingWidget::NextTitle, InData.CalcTime(), false);
}

void UMessagingWidget::AddTitle(const FToroSimpleMsg& InTitle, const bool bImmediate)
{
	if (!InTitle.IsValidData()) return;
	if (!Titles.IsEmpty() && Titles[Titles.Num() - 1].Message.EqualTo(InTitle.Message)) return;

	if (bImmediate)
	{
		GetWorld()->GetTimerManager().ClearTimer(TitleTimer);
		ShowTitle(InTitle);
	}
	else
	{
		Titles.Add(InTitle);
		if (!GetWorld()->GetTimerManager().IsTimerActive(TitleTimer))
		{
			NextTitle();
		}
	}
}

void UMessagingWidget::ClearInputRows()
{
	if (!InputEntryWidget || InputPreviews.IsEmpty()) return;

	TArray<TObjectPtr<UInputPreviewEntry>> Widgets;
	InputPreviews.GenerateValueArray(Widgets);
	InputPreviews.Empty();

	PlayAnimationReverse(InputRowAnim);
	FFlow::Delay(this, GetAnimDuration(InputRowAnim), [this, Widgets]()
	{
		for (const TObjectPtr<UInputPreviewEntry>& Widget : Widgets)
		{
			Widget->RemoveFromParent();
		}
	});
}

void UMessagingWidget::RemoveInputRow(const FName& InKey)
{
	if (!InputEntryWidget || !InputPreviews.Contains(InKey)) return;

	UInputPreviewEntry* Widget = InputPreviews[InKey];
	InputPreviews.Remove(InKey);

	const bool bLastRow = InputPreviews.Num() == 1;
	if (bLastRow) PlayAnimationReverse(InputRowAnim);
	FFlow::Delay(this, bLastRow ? GetAnimDuration(InputRowAnim) : 0.01f, [this, Widget]()
	{
		Widget->RemoveFromParent();
	});
}

void UMessagingWidget::AddInputRow(const FName& InKey, const FToroInputPrompt& InData)
{
	if (!InputEntryWidget || InputPreviews.Contains(InKey) || !InData.IsValidData()) return;

	PlayAnimationForward(InputRowAnim);
	if (UInputPreviewEntry* Entry = CreateWidget<UInputPreviewEntry>(this, InputEntryWidget))
	{
		Entry->InitData(InData);
		InputPreviews.Add(InKey, Entry);
	}
}

void UMessagingWidget::InitWidget()
{
	Super::InitWidget();
	NoticeText->SetText(FText::GetEmpty());
	TitleText->SetText(FText::GetEmpty());
}

void UMessagingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitAnim(InputRowAnim);
}
