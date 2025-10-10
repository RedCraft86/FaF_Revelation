// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameOverActor.h"
#include "GameOverWidget.h"
#include "UserInterface/ToroWidgetManager.h"
#include "Framework/ToroPlayerController.h"
#include "EngineUtils.h"

AGameOverActor::AGameOverActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

AGameOverActor* AGameOverActor::FindGameOver(const UObject* ContextObject, const TSubclassOf<AGameOverActor> Class, const FGameplayTag Tag)
{
	if (!ContextObject || !Class || !CharacterTags::IsValidTag(Tag)) return nullptr;
	const UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!World) return nullptr;
	for (AGameOverActor* GameOver : TActorRange(World, Class))
	{
		if (GameOver->GetCharacterID() == Tag)
		{
			return GameOver;
		}
	}
	return nullptr;
}

void AGameOverActor::ShowGameOver()
{
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->SetViewTarget(this);
	}
	if (UGameOverWidget* Widget = AToroWidgetManager::GetWidget<UGameOverWidget>(this))
	{
		Widget->ShowWidget(DisplayName, Description);
	}
}
