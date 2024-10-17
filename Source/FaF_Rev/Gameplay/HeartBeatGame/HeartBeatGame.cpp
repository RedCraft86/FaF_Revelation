// Copyright (C) RedCraft86. All Rights Reserved.

#include "HeartBeatGame.h"
#include "FRPlayerController.h"
#include "Core/MessageWidget.h"
#include "Core/GameWidget.h"
#include "FRGameMode.h"
#include "FRPlayer.h"

UHeartBeatGame::UHeartBeatGame() : Title(INVTEXT("Stay Calm!")), KeyA(EKeys::LeftMouseButton), KeyB(EKeys::RightMouseButton), MaxChances(5)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHeartBeatGame::StartGame(const TArray<FString> InSequence, const AFRCharacter* Enemy)
{
	LookComp = Enemy ? Enemy->GetPlayerLookTarget() : nullptr;
	Widget->StartGame(KeyA, KeyB, MaxChances, InSequence);
	Widget->SetTitle(Title);
}

void UHeartBeatGame::StartGameStr(const FString& InSequence, const AFRCharacter* Enemy)
{
	TArray<FString> Sequences;
	InSequence.ParseIntoArray(Sequences, TEXT("|"));
	StartGame(Sequences, Enemy);
}

void UHeartBeatGame::OnGameStart() const
{
	if (PlayerChar)
	{
		PlayerChar->SetLockOnTarget(LookComp);
		PlayerChar->AddLockFlag(Player::LockFlags::QTE);
		PlayerChar->GetGameMode()->GetWidget<UGameWidgetBase>()->SetWidgetHidden(true);
		PlayerChar->GetGameMode()->GetWidget<UMessageWidgetBase>()->SetWidgetHidden(true);
	}
}

void UHeartBeatGame::OnGameEnd(const bool bSuccess) const
{
	if (PlayerChar)
	{
		if (LookComp) PlayerChar->SetLockOnTarget(nullptr);
		PlayerChar->GetGameMode()->GetWidget<UGameWidgetBase>()->SetWidgetHidden(false);
		PlayerChar->GetGameMode()->GetWidget<UMessageWidgetBase>()->SetWidgetHidden(false);

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
		{
			PlayerChar->ClearLockFlag(Player::LockFlags::QTE);
		}, 1.0f, false);
	}

	bSuccess ? OnSuccess.Broadcast() : OnFail.Broadcast();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UHeartBeatGame::OnAnyKeyPressed(const FKey& InKey)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(KeyCooldown))
	{
		Widget->PressKey(InKey);
		GetWorld()->GetTimerManager().SetTimer(KeyCooldown, [](){}, 0.1f, false);
	}
}

void UHeartBeatGame::BeginPlay()
{
	Super::BeginPlay();
	PlayerChar = FRPlayer(this);
	if (PlayerChar)
	{
		PlayerChar->GetPlayerController()->OnAnyKeyPressed.AddDynamic(this, &UHeartBeatGame::OnAnyKeyPressed);
	}
	
	Widget = CreateWidget<UHeartBeatGameWidget>(GetWorld(), WidgetClass);
	Widget->Controller = this;

	if (!KeyA.IsValid()) KeyA = EKeys::LeftMouseButton;
	if (!KeyB.IsValid()) KeyB = EKeys::RightMouseButton;
}
