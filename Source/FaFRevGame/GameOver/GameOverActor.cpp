// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameOverActor.h"
#include "GameOverWidget.h"
#include "Player/PlayerCharacter.h"
#include "UserInterface/ToroWidgetManager.h"
#include "Framework/ToroPlayerController.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "EngineUtils.h"

AGameOverActor::AGameOverActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bDisplayOnRight(false)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

void AGameOverActor::InitiateGameOver(const UObject* ContextObject)
{
	const APlayerCharacter* Player = APlayerCharacter::Get<APlayerCharacter>(ContextObject);
	const FGameplayTag Enemy = Player ? Player->GetJumpscareEnemy() : FGameplayTag::EmptyTag;
	if (!CharacterTags::IsValidTag(Enemy) || Enemy == CharacterTags::TAG_Player.GetTag())
	{
		return;
	}

	for (AGameOverActor* GameOver : TActorRange<AGameOverActor>(Player->GetWorld()))
	{
		if (GameOver->GetCharacterID() == Enemy)
		{
			GameOver->ShowGameOver();
			return;
		}
	}
}

void AGameOverActor::ShowGameOver()
{
	UToroShortcutLibrary::SetCameraFade(this, 1.0f, FLinearColor::Black, true);
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->SetViewTarget(this);
	}
	UToroShortcutLibrary::StartCameraFade(this, 1.0f, 0.0f, 2.0f);
	if (UGameOverWidget* Widget = AToroWidgetManager::GetWidget<UGameOverWidget>(this))
	{
		Widget->ShowWidget(DisplayName, Description, bDisplayOnRight);
	}
}
