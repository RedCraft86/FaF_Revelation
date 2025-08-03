// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/DifficultyWidget.h"
#include "UserSettings/ToroUserSettings.h"
#include "UserWidgets/UserDialogWidget.h"
#include "Libraries/ToroGeneralUtils.h"
#include "Interfaces/ExitInterface.h"

UDifficultyWidget::UDifficultyWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), Difficulty(EGameDifficulty::Normal)
{
	bAutoActivate = false;
}

void UDifficultyWidget::OnConfirmClicked()
{
	DeactivateWidget();
	EGameDifficulty Initial = UToroUserSettings::Get()->GetDifficulty();
	if (Initial != Difficulty)
	{
		UUserDialogWidget::ShowDialog(this, {
			INVTEXT("Difficulty Change"),
			INVTEXT("Changing the difficulty requires reloading from the last checkpoint. Do you wish to reload?"),
			INVTEXT("Last Checkpoint"), INVTEXT("Cancel Change")},
			{}, [WeakThis = TWeakObjectPtr(this)](uint8 Button)
			{
				if (!WeakThis.IsValid()) return;
				if (Button == 0)
				{
					UToroUserSettings::Get()->SetDifficulty(WeakThis->Difficulty);
					UToroGeneralUtils::RestartLevel(WeakThis.Get());
				}
				else if (WeakThis->ParentUI)
				{
					IExitInterface::ReturnToWidget(WeakThis->ParentUI, WeakThis.Get());
					WeakThis->ParentUI = nullptr;
				}
			}
		);
	}
	else if (ParentUI)
	{
		IExitInterface::ReturnToWidget(ParentUI, this);
		ParentUI = nullptr;
	}
}

void UDifficultyWidget::SetDifficulty(const EGameDifficulty InDifficulty)
{
	if (Difficulty != InDifficulty)
	{
		Difficulty = InDifficulty;
		EasySelectVis->SetVisibility(ESlateVisibility::Collapsed);
		NormalSelectVis->SetVisibility(ESlateVisibility::Collapsed);
		HardSelectVis->SetVisibility(ESlateVisibility::Collapsed);
		switch (Difficulty)
		{
		case EGameDifficulty::Easy:
			EasySelectVis->SetVisibility(ESlateVisibility::HitTestInvisible);
			break;
		case EGameDifficulty::Normal:
			NormalSelectVis->SetVisibility(ESlateVisibility::HitTestInvisible);
			break;
		case EGameDifficulty::Hard:
			HardSelectVis->SetVisibility(ESlateVisibility::HitTestInvisible);
			break;
		}
	}
}

void UDifficultyWidget::InitWidget()
{
	Super::InitWidget();
	EasyButton->OnClicked.AddDynamic(this, &UDifficultyWidget::OnEasyClicked);
	NormalButton->OnClicked.AddDynamic(this, &UDifficultyWidget::OnNormalClicked);
	HardButton->OnClicked.AddDynamic(this, &UDifficultyWidget::OnHardClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &UDifficultyWidget::OnConfirmClicked);
}

void UDifficultyWidget::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	SetDifficulty(UToroUserSettings::Get()->GetDifficulty());
	if (!UToroUtilSettings::Get()->IsOnGameplayMap(this))
	{
		ReloadInfo->SetVisibility(ESlateVisibility::Collapsed);
	}
}
