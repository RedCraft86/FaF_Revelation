﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "Tutorial/TutorialWidget.h"
#include "Tutorial/TutorialDatabase.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroWidgetManager.h"
#include "UserWidgets/ExprTextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "EnhancedCodeFlow.h"

UTutorialWidget::UTutorialWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bLockNextBtn(true)
{
	ZOrder = 97;
	bAutoActivate = false;
}

void UTutorialWidget::QueueTutorial(const UObject* ContextObject, const FGameplayTag InTutorial)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UTutorialWidget* Widget = Manager->FindWidget<UTutorialWidget>())
		{
			Widget->AddTutorial(InTutorial);
		}
	}
}

void UTutorialWidget::QueueTutorials(const UObject* ContextObject, const TArray<FGameplayTag>& InTutorials)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UTutorialWidget* Widget = Manager->FindWidget<UTutorialWidget>())
		{
			Widget->AddTutorials(InTutorials);
		}
	}
}

void UTutorialWidget::ShowTutorial()
{
	bLockNextBtn = true;
	if (TutorialQueue.IsEmpty())
	{
		CustomWidgetBox->ClearChildren();
		TutorialTitle->SetText(FText::GetEmpty());
		TutorialText->SetText(FText::GetEmpty());
		TutorialImage->SetBrushFromTexture(DefaultImage, true);
		NextButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		NextButton->SetRenderOpacity(0.0f);
	}
	else if (const FTutorialEntry Data = UTutorialDatabase::Get(TutorialQueue[0]); Data.IsValidData())
	{
		if (Data.CustomWidget.IsNull())
		{
			TypeSwitch->SetActiveWidgetIndex(0);
			TutorialTitle->SetText(Data.Title);
			TutorialText->SetText(Data.Description, true);
			if (!Data.Image.IsNull())
			{
				TutorialImage->SetBrushFromTexture(Data.Image.LoadSynchronous(), true);
				TutorialImage->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			else
			{
				TutorialImage->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			TypeSwitch->SetActiveWidgetIndex(1);
			CustomWidgetBox->AddChild(CreateWidget(this,
				Data.CustomWidget.LoadSynchronous()));
		}

		bLockNextBtn = false;
		TutorialQueue.RemoveAt(0);
		PlayAnimation(TutorialAnim);
		PlayAnimation(NextBtnAnim);
	}
	else
	{
		TutorialQueue.RemoveAt(0);
		ShowTutorial();
	}
}

void UTutorialWidget::OnNextClicked()
{
	if (bLockNextBtn) return;
	PlayAnimationReverse(TutorialAnim);
	PlayAnimationReverse(NextBtnAnim, 2.0f);
	FFlow::Delay(this, 0.5f, [this]()
	{
		ShowTutorial();
	});
}

void UTutorialWidget::AddTutorial(const FGameplayTag& InTutorial)
{
	if (!TutorialTags::IsValidTag(InTutorial)) return;
	const bool bWorking = !TutorialQueue.IsEmpty();
	if (!TutorialQueue.Contains(InTutorial))
	{
		TutorialQueue.Add(InTutorial);
		if (!bWorking) ShowTutorial();
	}
}

void UTutorialWidget::AddTutorials(const TArray<FGameplayTag>& InTutorials)
{
	const uint8 StartNum = TutorialQueue.Num();
	for (const FGameplayTag& Tutorial : InTutorials)
	{
		if (TutorialTags::IsValidTag(Tutorial) && !TutorialQueue.Contains(Tutorial))
		{
			TutorialQueue.Add(Tutorial);
		}
	}
	if (StartNum == 0 && TutorialQueue.Num() > StartNum)
	{
		ShowTutorial();
	}
}

void UTutorialWidget::InitWidget()
{
	Super::InitWidget();
	NextButton->OnClicked.AddDynamic(this, &UTutorialWidget::OnNextClicked);
}

void UTutorialWidget::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		CachedInput = PC->GetInputModeData();
		PC->SetInputModeData({EGameInputMode::UI_Only, true,
			EMouseLockMode::LockAlways, true, this});
		PC->AddPauseRequest(this);
	}
	ShowTutorial();
}

void UTutorialWidget::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->RemovePauseRequest(this);
		PC->SetInputModeData(CachedInput);
	}
}
