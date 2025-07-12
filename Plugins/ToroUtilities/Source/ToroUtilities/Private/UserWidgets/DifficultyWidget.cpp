// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/DifficultyWidget.h"
#include "UserSettings/ToroUserSettings.h"
#include "Interfaces/ExitInterface.h"

UDifficultyWidget::UDifficultyWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), Difficulty(EGameDifficulty::Normal)
{
	bAutoActivate = false;
}

void UDifficultyWidget::OnConfirmClicked()
{
	DeactivateWidget();
	UToroUserSettings::Get()->SetDifficulty(Difficulty);

	if (!ParentUI) return;
	IExitInterface::ReturnToWidget(ParentUI, this);
	ParentUI = nullptr;
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
