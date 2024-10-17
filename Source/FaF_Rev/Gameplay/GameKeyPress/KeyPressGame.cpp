// Copyright (C) RedCraft86. All Rights Reserved.

#include "KeyPressGame.h"
#include "Core/GameWidget.h"
#include "Core/MessageWidget.h"
#include "Algo/RandomShuffle.h"
#include "Components/ProgressBar.h"
#include "KeyPressGameWidget.h"
#include "FRPlayerController.h"
#include "FRGameMode.h"
#include "FRPlayer.h"
#include "CharacterAI/FRCharacter.h"

UKeyPressGame::UKeyPressGame() : Title(INVTEXT("Stay Calm!")), RoundIdx(0), CountdownTime(0), bGameCompleted(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UKeyPressGame::StartGame(const TArray<int32>& InRounds, const AFRCharacter* Enemy)
{
	if (!Rounds.IsEmpty() || ValidKeys.IsEmpty()) return;
	if (GetWorld()->GetTimerManager().IsTimerActive(FadeOutTimer)) return;
	for (int32 i = 0; i < InRounds.Num(); i++)
	{
		FKeys Keys;
		Algo::RandomShuffle(ValidKeys);
		for (int32 j = 0; j < InRounds[i] * (i + 1); j++)
		{
			CountdownTime.Y++;
			Keys.Add(ValidKeys[FMath::RandRange(0, ValidKeys.Num() - 1)]);
		}

		if (!Keys.IsEmpty()) CountdownTime.Y++;
		Rounds.Add(Keys);
	}

	RoundIdx = 0;
	CountdownTime.X = CountdownTime.Y * 1.25f;
	LookComp = Enemy ? Enemy->GetPlayerLookTarget() : nullptr;
	
	StartNextRound();
	if (Widget) Widget->AddToViewport();
	if (PlayerChar)
	{
		PlayerChar->SetLockOnTarget(LookComp);
		PlayerChar->AddLockFlag(Player::LockFlags::QTE);
		PlayerChar->GetGameMode()->GetWidget<UGameWidgetBase>()->SetWidgetHidden(true);
		PlayerChar->GetGameMode()->GetWidget<UMessageWidgetBase>()->SetWidgetHidden(true);
	}
}

void UKeyPressGame::StartGameStr(const FString& InRounds, const class AFRCharacter* Enemy)
{
	TArray<FString> Parsed;
	InRounds.ParseIntoArray(Parsed, TEXT("|"));

	TArray<int> Rounds;
	for (const FString& Str : Parsed) Rounds.Add(FCString::Atoi(*Str));
	StartGame(Rounds, Enemy);
}

void UKeyPressGame::EndGame()
{
	RoundIdx = 0;
	CountdownTime = {0.0f, 0.0f};
	
	if (Widget) Widget->RemoveWidget();
	if (PlayerChar)
	{
		if (LookComp) PlayerChar->SetLockOnTarget(nullptr);
		PlayerChar->GetGameMode()->GetWidget<UGameWidgetBase>()->SetWidgetHidden(false);
		PlayerChar->GetGameMode()->GetWidget<UMessageWidgetBase>()->SetWidgetHidden(false);
		GetWorld()->GetTimerManager().SetTimer(FadeOutTimer, [this]()
		{
			PlayerChar->ClearLockFlag(Player::LockFlags::QTE);
		}, 1.0f, false);
	}

	LookComp = nullptr;
	bGameCompleted = false;
}

void UKeyPressGame::FailGame()
{
	if (!bGameCompleted)
	{
		bGameCompleted = true;
		Rounds.Empty();
		EndGame();

		OnFail.Broadcast();
	}
}

void UKeyPressGame::StartNextRound()
{
	if (Rounds.IsEmpty())
	{
		if (bGameCompleted)
		{
			bGameCompleted = true;
			EndGame();

			OnSuccess.Broadcast();
		}
		return;
	}
	
	while (!Rounds.IsEmpty() && Rounds[0].IsEmpty())
	{
		Rounds.RemoveAt(0);
		RoundIdx++;
	}
	
	if (!Rounds.IsEmpty())
	{
		if (!Widget) return;
		while (Widget->Buttons.Num() < RoundIdx + 1)
		{
			if (Rounds[0].IsValidIndex(Widget->Buttons.Num())) Widget->AddKey(Rounds[0][Widget->Buttons.Num()]);
			else break;
		}
	}
	else if (!bGameCompleted)
	{
		bGameCompleted = true;
		EndGame();

		OnSuccess.Broadcast();
	}
}

void UKeyPressGame::OnAnyKeyPressed(const FKey& InKey)
{
	if (Rounds.IsEmpty()) return;
	if (Rounds[0].IsEmpty()) StartNextRound();
	else if (Rounds[0][0] == InKey)
	{
		if (Widget) Widget->RemoveKey();
		Rounds[0].RemoveAt(0);
		StartNextRound();
	}
	else
	{
		if (Widget) Widget->WrongKey();
		
		CountdownTime.X--;
		if (CountdownTime.X <= 0.0f)
		{
			FailGame();
			Widget->Progress->SetPercent(0.0f);
		}
	}
}

void UKeyPressGame::BeginPlay()
{
	Super::BeginPlay();
	ValidKeys.RemoveAll([](const FKey& Element) -> bool { return !Element.IsValid(); });
	PlayerChar = FRPlayer(this);
	if (PlayerChar)
	{
		PlayerChar->GetPlayerController()->OnAnyKeyPressed.AddDynamic(this, &UKeyPressGame::OnAnyKeyPressed);
	}

	Widget = CreateWidget<UKeyPressGameWidget>(GetWorld(), WidgetClass);
	Widget->Controller = this;
}

void UKeyPressGame::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Rounds.IsEmpty() && CountdownTime.X > -0.5f)
	{
		CountdownTime.X -= DeltaTime;
		if (Widget)
		{
			Widget->Progress->SetPercent(FMath::FInterpConstantTo(Widget->Progress->GetPercent(),
				FMath::Max(0.0f, CountdownTime.X / CountdownTime.Y), DeltaTime, 1.0f));
		}
		if (CountdownTime.X <= 0.0f)
		{
			FailGame();
			Widget->Progress->SetPercent(0.0f);
		}
	}
}
