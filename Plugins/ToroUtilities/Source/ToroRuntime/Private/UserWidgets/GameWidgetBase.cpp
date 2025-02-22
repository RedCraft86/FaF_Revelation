// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/GameWidgetBase.h"
#include "Interaction/InteractionComponent.h"
#include "Characters/ToroPlayerBase.h"
#include "GameFramework/PlayerState.h"
#include "UserWidgets/ExprTextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"

UGameWidgetBase::UGameWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), DefaultIconSize(35.0f), bInteractionHidden(false)
{
	ZOrder = 55;
}

void UGameWidgetBase::SetInteractionHidden(const bool bInHidden)
{
	if (bInteractionHidden != bInHidden)
	{
		bInteractionHidden = bInHidden;
		if (InteractAnim)
		{
			bInteractionHidden ? PlayAnimationForward(InteractAnim) : PlayAnimationReverse(InteractAnim);
		}
		else
		{
			InteractWrapper->SetVisibility(bInteractionHidden ?
				ESlateVisibility::Hidden : ESlateVisibility::Visible);
		}
	}
}

void UGameWidgetBase::UpdateInteraction(const FInteractionData& InData)
{
	if (InData.Target)
	{
		Crosshair->SetVisibility(ESlateVisibility::Hidden);
		
		InteractLabel->SetText(InData.Info.Label);
		InteractLabel->SetVisibility(ESlateVisibility::Visible);
		UpdateInteractLabel(InteractLabel, InData.Info.LabelOffset,
			InteractBrush.GetImageSize().Size() / DefaultIconSize.Size());

		if (InData.Info.Interaction == EInteractableType::Held
			&& InData.Progress > 0.0f && !InData.bTriggered)
		{
			InteractIcon->SetVisibility(ESlateVisibility::Hidden);
			InteractProgress->SetVisibility(ESlateVisibility::Visible);
			UpdateInteractProgress(InData.Progress);
		}
		else
		{
			const bool bHasIcon = IsValid(InData.Info.Icon);
			if (InteractBrush.GetResourceObject() != (bHasIcon ? InData.Info.Icon : DefaultIcon))
			{
				InteractBrush.SetResourceObject(bHasIcon ? InData.Info.Icon : DefaultIcon);
				InteractBrush.SetImageSize(bHasIcon ? InData.Info.IconSize : DefaultIconSize);
				InteractIcon->SetBrush(InteractBrush);
			}

			InteractProgress->SetVisibility(ESlateVisibility::Hidden);
			InteractIcon->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		Crosshair->SetVisibility(ESlateVisibility::Visible);
		InteractIcon->SetVisibility(ESlateVisibility::Hidden);
		InteractProgress->SetVisibility(ESlateVisibility::Hidden);
		InteractLabel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGameWidgetBase::InitWidget()
{
	Super::InitWidget();
	WorldSettings = GetWorld()->GetWorldSettings();
	PlayerChar = AToroPlayerBase::Get(this);
}

bool UGameWidgetBase::ShouldBeHidden()
{
	if (Super::ShouldBeHidden() || IsValid(WorldSettings->GetPauserPlayerState()))
	{
		return true;
	}
	if (const AToroGameMode* GameMode = GetGameMode<AToroGameMode>())
	{
		return GameMode->Narrative->IsInDialogue();
	}
	return false;
}

void UGameWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	// TODO: Stamina
}
