﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "GuidePages/GuideWidgetBase.h"
#include "Framework/ToroWidgetManager.h"
#include "Animation/UMGSequencePlayer.h"
#include "UserWidgets/ExprTextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "ToroSettings.h"

UGuideWidgetBase::UGuideWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), ImageHeight(0.0f), bPrePauseState(false)
{
	ZOrder = 49;
	bAutoActivate = false;
}

void UGuideWidgetBase::QueuePage(const FGameplayTag& InPageID)
{
	if (InPageID.IsValid())
	{
		PageQueue.Enqueue(InPageID);
		StartBook();
	}
}

void UGuideWidgetBase::QueuePages(const TArray<FGameplayTag>& InPageIDs)
{
	for (const FGameplayTag& ID : InPageIDs)
	{
		if (ID.IsValid()) PageQueue.Enqueue(ID);
	}
	if (!PageQueue.IsEmpty()) StartBook();
}

void UGuideWidgetBase::StartBook()
{
	ActivateWidget();
	ProceedNextGuide();
}

void UGuideWidgetBase::ResetBook() const
{
	NextButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	NextButton->SetRenderOpacity(0.0f);
	CustomPageContainer->ClearChildren();
	LocalPageTitle->SetText(FText::GetEmpty());
	LocalPageText->SetText(FText::GetEmpty(), true);
	LocalImageContainer->ClearHeightOverride();
	LocalImageContainer->SetVisibility(ESlateVisibility::Collapsed);

	FSlateBrush Brush = LocalPageImage->GetBrush();
	Brush.SetResourceObject(DefaultImage);
	LocalPageImage->SetBrush(Brush);
}

void UGuideWidgetBase::ProceedNextGuide()
{
	ResetBook();
	FGameplayTag PageID;
	if (PageQueue.Dequeue(PageID))
	{
		if (const FGuidePageEntry Data = Database->Entries.FindRef(PageID); Data.IsValidData())
		{
			if (Data.CustomWidget.IsNull())
			{
				TypeSwitch->SetActiveWidgetIndex(0);
				LocalPageTitle->SetText(Data.Title);
				LocalPageText->SetText(Data.Description, true);
				if (!Data.Image.IsNull())
				{
					LocalImageContainer->SetVisibility(ESlateVisibility::HitTestInvisible);
					LocalImageContainer->SetHeightOverride(ImageHeight);
					FSlateBrush Brush = LocalPageImage->GetBrush();
					Brush.SetResourceObject(Data.Image.LoadSynchronous());
					Brush.SetImageSize(FVector2D{(float)Data.Image->GetSizeX(), (float)Data.Image->GetSizeY()});
					LocalPageImage->SetBrushFromTexture(Data.Image.LoadSynchronous(), true);
				}
			}
			else
			{
				TypeSwitch->SetActiveWidgetIndex(1);
				CustomPageContainer->AddChild(CreateWidget(this,
					Data.CustomWidget.LoadSynchronous()));
			}
			
			PlayAnimation(GuideFadeAnim);
			PlayAnimation(NextButtonAnim);
		}
		else
		{
			ProceedNextGuide();
		}
	}
	else
	{
		DeactivateWidget();
	}
}

void UGuideWidgetBase::OnNextClicked()
{
	NextButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PlayAnimationReverse(NextButtonAnim, 2.0f, true);
	if (UUMGSequencePlayer* Anim = PlayAnimationReverse(GuideFadeAnim))
	{
		Anim->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer&)
		{
			ProceedNextGuide();
		});
	}
}

void UGuideWidgetBase::InitWidget()
{
	Super::InitWidget();
	NextButton->OnClicked.AddDynamic(this, &UGuideWidgetBase::OnNextClicked);
	Database = UToroSettings::Get()->GuidePageDatabase.LoadSynchronous();
	bPrePauseState = UGameplayStatics::IsGamePaused(this);
	UGameplayStatics::SetGamePaused(this, true);
}

bool UGuideWidgetBase::ShouldBeHidden()
{
	return Super::ShouldBeHidden() || PageQueue.IsEmpty();
}

void UGuideWidgetBase::InternalProcessActivation()
{
	PreInputMode = GetOwningPlayer<AToroPlayerController>()->GetInputModeData();
	bPrePauseState = UGameplayStatics::IsGamePaused(this);
	UGameplayStatics::SetGamePaused(this, true);
	Super::InternalProcessActivation();
	ProceedNextGuide();
}

void UGuideWidgetBase::InternalProcessDeactivation()
{
	UGameplayStatics::SetGamePaused(this, bPrePauseState);
	GetOwningPlayer<AToroPlayerController>()->SetInputModeData(PreInputMode);
	Super::InternalProcessDeactivation();
}

void UGuideWidgetBase::QueueGuidePage(const UObject* ContextObject, const FGameplayTag InPageID)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UGuideWidgetBase* Widget = Manager->FindWidget<UGuideWidgetBase>())
		{
			Widget->QueuePage(InPageID);
		}
	}
}

void UGuideWidgetBase::QueueGuidePages(const UObject* ContextObject, const TArray<FGameplayTag>& InPageIDs)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UGuideWidgetBase* Widget = Manager->FindWidget<UGuideWidgetBase>())
		{
			Widget->QueuePages(InPageIDs);
		}
	}
}
