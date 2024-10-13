// Copyright (C) RedCraft86. All Rights Reserved.

#include "HeartBeatGameWidget.h"
#include "VirtualMouseWidget.h"
#include "Animation/UMGSequencePlayer.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UHeartBeatGameButton::ButtonClicked()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	if (!bPressed)
	{
		Cooldown = 0.2f;
		PlayAnimation(SuccessAnim);
		if (Parent) Parent->OnCorrectKey();
	}
	else
	{
		PlayAnimation(FailAnim);
		if (Parent) Parent->OnWrongKey();
	}
}

void UHeartBeatGameButton::NativeConstruct()
{
	Super::NativeConstruct();
	Button->OnClicked.AddUniqueDynamic(this, &UHeartBeatGameButton::ButtonClicked);
}

void UHeartBeatGameButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (Cooldown > 0.0f)
	{
		Cooldown -= InDeltaTime;
		if (Cooldown <= 0.0f)
		{
			SetVisibility(ESlateVisibility::Visible);
		}
	}
	
	if (!Parent || bStopTick) return;
	if (MyGeometry.LocalToAbsolute(FVector2D::ZeroVector).X + MyGeometry.GetAbsoluteSize().X + 5.0f < Parent->VirtualMouse->GetPosition().X)
	{
		if (!bPressed)
		{
			PlayAnimation(FailAnim);
			if (Parent) Parent->OnMissedKey();
			SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		bStopTick = true;
	}
}

void UHeartBeatGameWidget::StartGame(const FKey InKeyA, const FKey InKeyB, const uint8 InMaxChances, const TArray<FString> InSequence)
{
	if (bInGame || InSequence.IsEmpty() || InMaxChances <= 0 || !InKeyA.IsValid() || !InKeyB.IsValid())
	{
		return;
	}

	bInGame = true;
	Sequence = InSequence;
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Chances = InMaxChances;
	MaxChance->SetText(FText::FromString(FString::FromInt(InMaxChances)));
	CurChance->SetText(MaxChance->GetText());
			
	KeyRange = {InKeyA, InKeyB};
	KeyImageA->SetBrush(GetKeyIcon(InKeyA.GetFName()).Brush);
	KeyImageA->SetBrush(GetKeyIcon(InKeyA.GetFName()).Brush);
			
	Buttons.Empty();
	Container->ClearChildren();

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UHeartBeatGameWidget::AddChildEntries, 0.05f, false);

	PlayAnimation(FadeAnim);
}

void UHeartBeatGameWidget::OnWrongKey()
{
	Chances--;
	CurChance->SetText(FText::FromString(FString::FromInt(Chances)));
	PlayAnimation(WrongAnim);
	ProcessNextButton();
}

void UHeartBeatGameWidget::OnMissedKey()
{
	Buttons.RemoveAt(0);
	OnWrongKey();
}

void UHeartBeatGameWidget::OnCorrectKey()
{
	Buttons.RemoveAt(0);
	ProcessNextButton();
}

void UHeartBeatGameWidget::AddChildEntries()
{
	EmptySpaceImage.ImageSize.X = GetCachedGeometry().GetAbsoluteSize().X * 0.5f;
	UImage* InitialIMG = WidgetTree->ConstructWidget<UImage>();
	InitialIMG->SetBrush(EmptySpaceImage);
	Container->AddChild(InitialIMG);
			
	for (const FString& Element : Sequence)
	{
		if (Element.ToLower().StartsWith(TEXT("key")))
		{
			UHeartBeatGameButton* BTN = WidgetTree->ConstructWidget<UHeartBeatGameButton>(ButtonClass);
			BTN->bAltKey = Element.ToLower().EndsWith(TEXT("b"));
			BTN->Parent = this;
			Buttons.Add(BTN);
			Container->AddChild(BTN);
		}
		else if (const float AsFloat = FCString::Atof(*Element); AsFloat > 0.01f)
		{
			EmptySpaceImage.ImageSize.X = AsFloat;
			UImage* IMG = WidgetTree->ConstructWidget<UImage>();
			IMG->SetBrush(EmptySpaceImage);
			Container->AddChild(IMG);
		}
	}

	EmptySpaceImage.ImageSize.X = GetCachedGeometry().GetAbsoluteSize().X;
	UImage* EndIMG = WidgetTree->ConstructWidget<UImage>();
	EndIMG->SetBrush(EmptySpaceImage);
	Container->AddChild(EndIMG);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UHeartBeatGameWidget::StartScrollAnim, 0.05f, false);
}

void UHeartBeatGameWidget::StartScrollAnim()
{
	PlayAnimation(ScrollAnim, 0, 1, EUMGSequencePlayMode::Forward, ScrollRate *
		FMath::Min(1.0f, GetCachedGeometry().GetLocalSize().X / Container->GetCachedGeometry().GetLocalSize().X));
}

void UHeartBeatGameWidget::RemoveWidget()
{
	if (UUMGSequencePlayer* Player = PlayAnimation(FadeAnim, 0, 1, EUMGSequencePlayMode::Reverse))
	{
		Player->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer&)
		{
			StopAnimation(ScrollAnim);
			RemoveFromParent();
		});
	}
}

void UHeartBeatGameWidget::ProcessNextButton()
{
	if (Chances <= 0)
	{
		bInGame = false;
		Buttons.Empty();
		OnFail.Broadcast();
		RemoveWidget();
		return;
	}
	if (Buttons.IsEmpty())
	{
		bInGame = false;
		OnSuccess.Broadcast();
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
		{
			RemoveWidget();
		}, 1.0f, false);
		return;
	}
	if (bLastKey != Buttons[0]->bAltKey)
	{
		bLastKey = Buttons[0]->bAltKey;
		CurrentKey = bLastKey ? KeyRange.Value : KeyRange.Key;
		PlayAnimation(SwapAnim, 0, 1, bLastKey ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
	}
}

void UHeartBeatGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Collapsed);
	BackingButton->OnClicked.AddUniqueDynamic(this, &UHeartBeatGameWidget::OnWrongKey);
	bInGame = false;
}
