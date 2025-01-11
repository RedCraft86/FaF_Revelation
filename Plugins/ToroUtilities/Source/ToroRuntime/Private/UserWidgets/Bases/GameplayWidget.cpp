// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/Bases/GameplayWidget.h"
#include "Interaction/InteractionComponent.h"
#include "UserWidgets/ExprTextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"

UGameplayWidgetBase::UGameplayWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), DefaultIconSize(35.0f), bInteractHidden(false)
{
	bAutoAdd = true;
	ZOrder = 1;
}

void UGameplayWidgetBase::SetInteractionHidden(const bool bInHidden)
{
	if (bInteractHidden != bInHidden)
	{
		bInteractHidden = bInHidden;
		if (IsValid(InteractHideAnim))
		{
			if (bInteractHidden)
			{
				PlayAnimationForward(InteractHideAnim);
			}
			else
			{
				PlayAnimationReverse(InteractHideAnim);
			}
		}
		else
		{
			InteractWrapper->SetVisibility(bInteractHidden ?
				ESlateVisibility::Hidden : ESlateVisibility::Visible);
		}
	}
}

void UGameplayWidgetBase::UpdateInteraction()
{
	if (Component)
	{
		const FInteractionData& Data = Component->GetCacheData();
		if (!Data.Target)
		{
			BaseIcon->SetVisibility(ESlateVisibility::Visible);
			CustomIcon->SetVisibility(ESlateVisibility::Hidden);
			ProgressWidget->SetVisibility(ESlateVisibility::Hidden);
			Label->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			BaseIcon->SetVisibility(ESlateVisibility::Hidden);
			
			Label->SetText(Data.Info.Label);
			Label->SetVisibility(ESlateVisibility::Visible);
			UpdateLabelPos(Label, Data.Info.LabelOffset,
				IconBrush.GetImageSize().Size() / DefaultIconSize.Size());
			
			if (Data.Info.Interaction == EInteractableType::Held
				&& Data.Progress > 0.0f && !Data.bTriggered)
			{
				CustomIcon->SetVisibility(ESlateVisibility::Hidden);
				ProgressWidget->SetVisibility(ESlateVisibility::Visible);
				UpdateProgress(Data.Progress);
			}
			else
			{
				const bool bHasIcon = IsValid(Data.Info.Icon);
				if (IconBrush.GetResourceObject() != (bHasIcon ? Data.Info.Icon : DefaultIcon))
				{
					IconBrush.SetResourceObject(bHasIcon ? Data.Info.Icon : DefaultIcon);
					IconBrush.SetImageSize(bHasIcon ? Data.Info.IconSize : DefaultIconSize);
					CustomIcon->SetBrush(IconBrush);
				}

				ProgressWidget->SetVisibility(ESlateVisibility::Hidden);
				CustomIcon->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UGameplayWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
	IconBrush = CustomIcon->GetBrush();
}
