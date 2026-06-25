// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GameStageManager.h"
#include "GameStageNode.h"
#include "GameState/SessionState.h"
#include "Kismet/GameplayStatics.h"
#include "AsyncGameplayMessageSystem.h"
#include "AsyncMessageWorldSubsystem.h"
#include "Helpers/LatentInfo.h"
#include "GameRuntime.h"

UGameStageManager::UGameStageManager(): bTransition(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

UGameStageManager* UGameStageManager::Get(const UObject* ContextObject)
{
	ASessionState* State = ASessionState::Get<ASessionState>(ContextObject);
	return IsValid(State) ? State->GetStageManager() : nullptr;
}

UE5Coro::TCoroutine<> UGameStageManager::LoadGameStage(const UGameStageNode* InStage)
{
	if (bTransition)
	{
		UE_LOG(LogGameRuntime, Warning, TEXT("Undergoing active transition; %s rejected."), *GetNameSafe(InStage))
		co_return;
	}

	bTransition = true;

	if (!InStage || ActiveStage == InStage || InStage->Levels.IsEmpty())
	{
		UE_LOG(LogGameRuntime, Warning, TEXT("Failed to load game stage %s"), *GetNameSafe(InStage))
		co_return;
	}

	for (auto It = LoadedLevels.CreateIterator(); It; ++It)
	{
		if (!It->IsNull() && !InStage->Levels.Contains(*It))
		{
			UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, *It, FLatentInfo::Make(), false);
		}

		It.RemoveCurrent();
	}

	TArray<UE5Coro::TCoroutine<>> Tasks;
	Tasks.Reserve(InStage->Levels.Num());

	for (const auto& [Level, LoadType] : InStage->Levels)
	{
		Tasks.Add(LoadLevel(Level, LoadType == EStageLevelLoadType::LoadAndShow));
	}

	co_await UE5Coro::WhenAll(Tasks);

	// TODO: Player states and flags?

	// Force the teleport if this is the first stage that's being loaded
	InStage->Teleporter.TeleportTo(!ActiveStage.IsValid());

	co_await UE5Coro::Latent::RealSeconds(0.5f);

	ActiveStage = InStage;
	bTransition = false;

	if (ActiveStage->StageEvent.IsValid() && ActiveStage->StageEvent != TAG_GameStage.GetTag())
	{
		const TSharedPtr<FAsyncGameplayMessageSystem> System = UAsyncMessageWorldSubsystem
			::GetSharedMessageSystem<FAsyncGameplayMessageSystem>(GetWorld());
		System->QueueMessageForBroadcast(FAsyncMessageId(ActiveStage->StageEvent));
	}
}

UE5Coro::TCoroutine<> UGameStageManager::LoadLevel(const TSoftObjectPtr<UWorld>& Level, const bool bVisible)
{
	if (!Level.IsNull())
	{
		co_await UE5Coro::Latent::ChainEx(&UGameplayStatics::LoadStreamLevelBySoftObjectPtr,
			this, std::cref(Level), bVisible, false, std::placeholders::_2);
	}
}
