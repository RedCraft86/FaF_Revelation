// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inspection/InspectionWidget.h"
#include "Framework/ToroPlayerController.h"

UInspectionWidget::UInspectionWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bSecretFound(false)
{
	ZOrder = 96;
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

void UInspectionWidget::LoadData(const UInventoryAsset* InArchive, bool bSecretKnown)
{
	if (InArchive && InArchive->IsValidArchive())
	{
		InitAnim(SecretAnim);
		TitleText->SetText(InArchive->DisplayName);
		DescText->SetText(InArchive->Description);
		SecretText->SetText(InArchive->SecretText);
		ContentText->SetText(InArchive->ContentText);

		bSecretFound = bSecretKnown;
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
		CachedInput = PC->GetInputModeData();
		PC->SetInputModeData({EGameInputMode::GameAndUI, true});
	}
}

void UInspectionWidget::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->SetInputModeData(CachedInput);
	}
}
