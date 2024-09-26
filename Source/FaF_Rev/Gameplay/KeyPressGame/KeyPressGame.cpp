// Copyright (C) RedCraft86. All Rights Reserved.

#include "KeyPressGame.h"
#include "Core/GameWidget.h"
#include "Core/MessageWidget.h"
#include "Algo/RandomShuffle.h"
#include "KeyPressGameWidget.h"
#include "FRPlayerController.h"
#include "FRGameMode.h"
#include "FRPlayer.h"

UKeyPressGame::UKeyPressGame() : RoundIdx(0)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UKeyPressGame::StartGame(const TArray<int32>& InRounds)
{
	if (!Rounds.IsEmpty() || ValidKeys.IsEmpty()) return;
	for (int32 i = 0; i < InRounds.Num(); i++)
	{
		FKeys Keys;
		Algo::RandomShuffle(ValidKeys);
		for (int32 j = 0; j < InRounds[i] * (i + 1); j++)
		{
			Keys.Add(ValidKeys[FMath::RandRange(0, ValidKeys.Num() - 1)]);
		}
		
		Rounds.Add(Keys);
	}
	
	RoundIdx = 0;
	if (Widget) Widget->AddToViewport();
	StartNextRound();
	if (PlayerChar)
	{
		PlayerChar->AddLockFlag(Player::LockFlags::KeyPressGame);
		PlayerChar->GetGameMode()->GetWidget<UGameWidgetBase>()->SetWidgetHidden(true);
		PlayerChar->GetGameMode()->GetWidget<UMessageWidgetBase>()->SetWidgetHidden(true);
	}
}

void UKeyPressGame::EndGame()
{
	RoundIdx = 0;
	if (Widget) Widget->RemoveWidget();
	if (PlayerChar)
	{
		PlayerChar->ClearLockFlag(Player::LockFlags::KeyPressGame);
		PlayerChar->GetGameMode()->GetWidget<UGameWidgetBase>()->SetWidgetHidden(false);
		PlayerChar->GetGameMode()->GetWidget<UMessageWidgetBase>()->SetWidgetHidden(false);
	}
}

void UKeyPressGame::StartNextRound()
{
	if (Rounds.IsEmpty())
	{
		EndGame();
		return;
	}
	
	if (Rounds[0].IsEmpty())
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
	else
	{
		EndGame();
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
}
