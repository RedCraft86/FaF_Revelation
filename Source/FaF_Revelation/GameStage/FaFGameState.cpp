// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "FaFGameState.h"
#include "TimerManager.h"
#include "FlowComponent.h"
#include "GameStageData.h"
#include "FaFRevSettings.h"
#include "Kismet/GameplayStatics.h"
#include "SaveObjects/GameSaveObject.h"
#include "Framework/ToroWorldSettings.h"
#include "AsyncMessageWorldSubsystem.h"
#include "AsyncGameplayMessageSystem.h"
#include "Objective/ObjectiveManager.h"
#include "Helpers/LatentInfo.h"

AFaFGameState::AFaFGameState()
{
	ObjectiveManager = CreateDefaultSubobject<UObjectiveManager>("ObjectiveManager");
}

UE5Coro::TCoroutine<> AFaFGameState::LoadGameStage(const UGameStageData* InStage)
{
	if (!InStage || CurrentStage == InStage || InStage->Levels.IsEmpty())
	{
		UE_LOG(LogGameState, Error, TEXT("Failed to load game stage %s"), *GetNameSafe(InStage))
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

	for (const auto& [Level, bVisible] : InStage->Levels)
	{
		Tasks.Add(LoadLevel(Level, bVisible));
	}

	co_await UE5Coro::WhenAll(Tasks);

	// TODO: Player states and flags?

	if ((!CurrentStage.IsValid() || !InStage->bContinuous) && InStage->Teleport.LoadSynchronous())
	{
		InStage->Teleport->TeleportPlayer();
	}

	CurrentStage = InStage;

	co_await UE5Coro::Latent::RealSeconds(1.0);

	ObjectiveManager->StartObjectives(InStage->Objectives);

	if (InStage->StageEvent.IsValid() && InStage->StageEvent != TAG_GameStage.GetTag())
	{
		const TSharedPtr<FAsyncGameplayMessageSystem> System = UAsyncMessageWorldSubsystem::
		   GetSharedMessageSystem<FAsyncGameplayMessageSystem>(FWorldGetter::Get(this));
		System->QueueMessageForBroadcast(FAsyncMessageId(InStage->StageEvent));
	}
}

UE5Coro::TCoroutine<> AFaFGameState::LoadLevel(const TSoftObjectPtr<UWorld>& Level, const bool bVisible)
{
	if (!Level.IsNull())
	{
		co_await UE5Coro::Latent::ChainEx(
			&UGameplayStatics::LoadStreamLevelBySoftObjectPtr, 
			this, std::cref(Level), bVisible, false, std::placeholders::_2);
	}
}

void AFaFGameState::FinishLoadingGame() const
{
	const AToroWorldSettings* WorldSettings = AToroWorldSettings::Get(this);
	if (ensureAlways(WorldSettings))
	{
		WorldSettings->GetGameFlowManager()->StartRootFlow();
	}
}

void AFaFGameState::BeginPlay()
{
	Super::BeginPlay();
	if (UFaFRevSettings::Get()->IsInMapType(this, EGameMapType::Gameplay))
	{
		UGameSaveObject* GameSave = UGameSaveObject::Get(this);
		UE5Coro::TCoroutine<EToroSaveLoadStatus> LoadResult = GameSave->LoadFromFile(0);
		LoadResult.ContinueWith([WeakThis = TWeakObjectPtr(this), GameSave](const EToroSaveLoadStatus Status)
		{
			if (WeakThis.IsValid() && ensureAlways(Status == EToroSaveLoadStatus::Succeeded))
			{
				GameSave->InitPlayTimer(WeakThis->GetWorld());
				WeakThis->FinishLoadingGame();
			}
		});
	}
}
