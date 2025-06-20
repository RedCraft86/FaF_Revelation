// Copyright (C) RedCraft86. All Rights Reserved.

#include "InspectionWidget.h"
#include "Components/PanelWidget.h"
#include "Framework/ToroPlayerController.h"
#include "Inventory/InventoryItemData.h"

UInspectionWidget::UInspectionWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bSecretFound(false)
{
	ZOrder = 95;
	bAutoActivate = false;
}

void UInspectionWidget::MarkSecretFound()
{
	if (!bSecretFound)
	{
		bSecretFound = true;
		PlayAnimation(SecretAnim);
		UGameplayStatics::PlaySound2D(this, SecretSound);
	}
}

void UInspectionWidget::LoadData(const UInvItemData* InData, const bool bKnowsSecret)
{
	if (InData && InData->ItemType == EInvItemType::Archive)
	{
		InitAnim(SecretAnim);
		TitleText->SetText(InData->DisplayName);
		ContentText->SetText(InData->Description);
		SecretText->SetText(FText::Format(INVTEXT("{0}: {1}"),
			InData->SecretLabel, InData->SecretText));

		bSecretFound = bKnowsSecret;
		if (bSecretFound)
		{
			PlayAnimation(SecretAnim, 0.0f, 1,
				EUMGSequencePlayMode::Forward, 10.0f, false);
		}
	}
}

void UInspectionWidget::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		CachedInputMode = PC->GetInputModeData();
		PC->SetInputModeData({EGameInputMode::GameAndUI, true});
	}
}

void UInspectionWidget::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->SetInputModeData(CachedInputMode);
	}
}
