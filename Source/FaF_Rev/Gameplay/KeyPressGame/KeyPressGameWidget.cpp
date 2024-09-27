// Copyright (C) RedCraft86. All Rights Reserved.

#include "KeyPressGameWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UKeyPressGameButton::WrongAnim()
{
	PlayAnimation(FailAnim);
}

void UKeyPressGameButton::RemoveWidget()
{
	if (UUMGSequencePlayer* Player = PlayAnimation(SuccessAnim))
	{
		Player->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer&)
		{
			RemoveFromParent();
		});
	}
}

void UKeyPressGameButton::Setup(const FKey& InKey)
{
	Key = InKey;
	KeyImage->SetBrush(GetKeyIcon(Key.GetFName()).Brush);
	PlayAnimation(ShowAnim);
}

void UKeyPressGameWidget::WrongKey()
{
	PlayAnimation(FailAnim);
	if (!Buttons.IsEmpty()) Buttons[0]->WrongAnim();
}

void UKeyPressGameWidget::RemoveKey()
{
	if (!Buttons.IsEmpty())
	{
		Buttons[0]->RemoveWidget(); 
		Buttons.RemoveAt(0);
	}
}

void UKeyPressGameWidget::AddKey(const FKey& InKey)
{
	if (InKey.IsValid() && ButtonClass)
	{
		if (UKeyPressGameButton* Widget = WidgetTree->ConstructWidget<UKeyPressGameButton>(ButtonClass))
		{
			Buttons.Add(Widget);
			ButtonContainer->AddChild(Widget);
			Widget->Setup(InKey);
		}
	}
}

void UKeyPressGameWidget::RemoveWidget()
{
	if (UUMGSequencePlayer* Player = PlayAnimation(FadeAnim))
	{
		Player->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer&)
		{
			Buttons.Empty();
			Progress->SetPercent(1.0f);
			ButtonContainer->ClearChildren();
			RemoveFromParent();
		});
	}
}

void UKeyPressGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(FadeAnim);
}
