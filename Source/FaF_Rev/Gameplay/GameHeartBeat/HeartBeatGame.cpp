// Copyright (C) RedCraft86. All Rights Reserved.

#include "HeartBeatGame.h"
#include "Helpers/LoggingHelpers.h"
#include "CharacterAI/FRCharacter.h"
#include "Core/MessageWidget.h"
#include "Core/GameWidget.h"
#include "FRGameMode.h"
#include "FRPlayer.h"

bool IsValidStr(const FString& InStr)
{
	if (InStr.IsEmpty()) return false;
	for (const TCHAR& Char : InStr)
	{
		if (Char != '0' && Char != '1' && Char != '2') return false;
	}

	return true;
}

UHeartBeatGame::UHeartBeatGame() : PrimaryKey(EKeys::LeftMouseButton), SecondaryKey(EKeys::RightMouseButton), bIsPlaying(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHeartBeatGame::StartGame(const FString& Sequence, const AFRCharacter* Enemy)
{
	if (IsValidStr(Sequence))
	{
		UE_KLOG_WARNING(1.0f, TEXT("Sequence String [%s] contains invalid characters. It must only consist of 0, 1, and 2."), *Sequence);
		return;
	}

	bIsPlaying = true;
	LookComp = Enemy ? Enemy->GetPlayerLookTarget() : nullptr;
	
	if (Widget) Widget->AddWidget(Sequence);
	if (PlayerChar)
	{
		PlayerChar->SetLockOnTarget(LookComp);
		PlayerChar->AddLockFlag(Player::LockFlags::QTE);
		PlayerChar->GetGameMode()->GetWidget<UGameWidgetBase>()->SetWidgetHidden(true);
		PlayerChar->GetGameMode()->GetWidget<UMessageWidgetBase>()->SetWidgetHidden(true);
	}
}

void UHeartBeatGame::OnGameFinished(const bool bSuccess)
{
	if (PlayerChar)
	{
		if (LookComp) PlayerChar->SetLockOnTarget(nullptr);
		PlayerChar->GetGameMode()->GetWidget<UGameWidgetBase>()->SetWidgetHidden(false);
		PlayerChar->GetGameMode()->GetWidget<UMessageWidgetBase>()->SetWidgetHidden(false);
		GetWorld()->GetTimerManager().SetTimer(FadeOutTimer, [this]()
		{
			bIsPlaying = false;
			PlayerChar->ClearLockFlag(Player::LockFlags::QTE);
		}, 1.0f, false);
	}

	bSuccess ? OnSuccess.Broadcast() : OnFail.Broadcast();
	LookComp = nullptr;
}

void UHeartBeatGame::BeginPlay()
{
	Super::BeginPlay();
	PlayerChar = FRPlayer(this);
	Widget = CreateWidget<UHeartBeatGameWidget>(GetWorld(), WidgetClass);

	if (!PrimaryKey.IsValid()) PrimaryKey = EKeys::LeftMouseButton;
	if (!SecondaryKey.IsValid()) SecondaryKey = EKeys::RightMouseButton;
}
