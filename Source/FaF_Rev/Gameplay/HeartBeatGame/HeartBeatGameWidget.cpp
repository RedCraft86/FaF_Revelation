// Copyright (C) RedCraft86. All Rights Reserved.

#include "HeartBeatGameWidget.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/HorizontalBox.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "HeartBeatGame.h"

void UHeartBeatGameButton::ButtonClicked(const bool bSuccess)
{
	if (bPressed) return;
	PlayAnimation(bSuccess ? SuccessAnim : FailAnim);
	if (Parent) bSuccess ? Parent->OnCorrectKey() : Parent->OnMissedKey();
	bPressed = true;
}

void UHeartBeatGameButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!Parent || bPressed || bStopTick) return;
	if (MyGeometry.LocalToAbsolute(FVector2D::ZeroVector).X + MyGeometry.GetAbsoluteSize().X + 5.0f < Parent->GetCursorPos().X)
	{
		ButtonClicked(false);
		bStopTick = true;
	}
}

void UHeartBeatGameWidget::StartGame(const FKey InKeyA, const FKey InKeyB, const uint8 InMaxChances, const TArray<FString>& InSequence)
{
	if (bInGame || InSequence.IsEmpty() || InMaxChances <= 0 || !InKeyA.IsValid() || !InKeyB.IsValid())
	{
		return;
	}

	AddToViewport(100);

	bInGame = true;
	Sequence = InSequence;
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (Controller) Controller->OnGameStart();

	Chances = InMaxChances;
	MaxChance->SetText(FText::FromString(FString::FromInt(InMaxChances)));
	CurChance->SetText(MaxChance->GetText());

	CurrentKey = InKeyA;
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
	if (Buttons.Num() > 0) Buttons.RemoveAt(0);
	OnWrongKey();
}

void UHeartBeatGameWidget::OnCorrectKey()
{
	if (Buttons.Num() > 0) Buttons.RemoveAt(0);
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

FVector2D UHeartBeatGameWidget::GetCursorPos()
{
	return VirtualCursor->GetCachedGeometry().LocalToAbsolute(FVector2D::ZeroVector);
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
		if (Controller) Controller->OnGameEnd(false);
		RemoveWidget();
		return;
	}
	if (Buttons.IsEmpty())
	{
		bInGame = false;
		if (Controller) Controller->OnGameEnd(true);
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
		{
			RemoveWidget();
		}, 1.0f, false);
		return;
	}
	if (bAltKey != Buttons[0]->bAltKey)
	{
		bAltKey = Buttons[0]->bAltKey;
		FKey LastKey = CurrentKey;
		CurrentKey = bAltKey ? KeyRange.Value : KeyRange.Key;
		PlayAnimation(SwapAnim, 0, 1, bAltKey ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
	}
}

void UHeartBeatGameWidget::PressKey(const FKey& InKey)
{
	if (!bInGame) return;

	bool bFoundElem = false;
	const FVector2D Pos = GetCursorPos();
	for (TArray<UWidget*> Children = Container->GetAllChildren(); UWidget* Child : Children)
	{
		if (Child && Child->IsA<UHeartBeatGameButton>() && Child->GetCachedGeometry().IsUnderLocation(Pos))
		{
			Cast<UHeartBeatGameButton>(Child)->ButtonClicked(InKey == CurrentKey);
			bFoundElem = true;
			break;
		}
	}

	if (!bFoundElem) OnWrongKey();
}

void UHeartBeatGameWidget::SetTitle(const FText& InTitle) const
{
	TitleText->SetText(InTitle);
}
