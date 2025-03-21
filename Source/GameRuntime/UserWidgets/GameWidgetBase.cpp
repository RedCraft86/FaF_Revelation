// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/GameWidgetBase.h"
#include "Interaction/InteractionComponent.h"
#include "Framework/ToroPlayerCharacter.h"
#include "UserWidgets/ExprTextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"
#include "ToroSettings.h"

UGameWidgetBase::UGameWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), StaminaBarSpeed(0.1f, 10.0f), DefaultIconSize(35.0f), bInteraction(false)
{
	ZOrder = 40;
}

void UGameWidgetBase::UpdateInteraction(const bool bEnabled, const FInteractionData& InData)
{
	if (bInteraction != bEnabled)
	{
		bInteraction = bEnabled;
		if (InteractHideAnim)
		{
			bInteraction ? PlayAnimationReverse(InteractHideAnim) : PlayAnimationForward(InteractHideAnim);
		}
		else
		{
			InteractWrapper->SetVisibility(bInteraction ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
	}

	if (!bInteraction)
		return;

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

void UGameWidgetBase::UpdateStamina(const float DeltaTime, const float Percent, const float ChangeDelta, const bool bPunished) const
{
	StaminaBar->SetPercent(FMath::FInterpConstantTo(StaminaBar->GetPercent(),
		Percent, DeltaTime, FMath::Abs(ChangeDelta) * StaminaBarSpeed.X));

	StaminaBar->SetRenderOpacity(FMath::GetMappedRangeValueClamped(FVector2D(0.85f, 1.0f),
		FVector2D(1.0f, Percent >= 0.0f ? 0.05f : 0.0f), StaminaBar->GetPercent()));

	StaminaBar->SetFillColorAndOpacity(FMath::CInterpTo(StaminaBar->GetFillColorAndOpacity(),
		bPunished ? FLinearColor::Red : FLinearColor::White, DeltaTime, StaminaBarSpeed.Y));
}

void UGameWidgetBase::InitWidget()
{
	Super::InitWidget();
	Player = AToroPlayerCharacter::Get<AGamePlayerBase>(this);
	Player->OnStaminaUpdate.BindUObject(this, &UGameWidgetBase::UpdateStamina);
	Player->Interaction->OnUpdateInfo.BindUObject(this, &UGameWidgetBase::UpdateInteraction);
}

bool UGameWidgetBase::ShouldBeHidden()
{
	return Super::ShouldBeHidden() || GetOwningPlayer()->IsPaused()
		|| (Player && Player->GetNarrative()->IsInDialogue());
}

bool UGameWidgetBase::CanCreateWidget() const
{
	return UToroSettings::Get()->IsOnGameplayMap(this);
}
