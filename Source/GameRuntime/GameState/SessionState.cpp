// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "SessionState.h"
#include "GameSettings.h"
#include "GameFlagManager.h"
#include "Helpers/LatentInfo.h"
#include "GameStage/GameStageManager.h"
#include "Inventory/InventoryManager.h"
#include "SaveSystem/ToroSaveManager.h"
#include "SaveObjects/SessionSaveObject.h"
#include "Framework/ToroWorldSettings.h"
#include "FlowComponent.h"
#include "GlobalState.h"

ASessionState::ASessionState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	PrimaryActorTick.TickInterval = 1.0f;

	SessionFlags = CreateDefaultSubobject<UGameFlagManager>(TEXT("SessionFlags"));
	StageManager = CreateDefaultSubobject<UGameStageManager>(TEXT("StageManager"));
	InventoryManager = CreateDefaultSubobject<UInventoryManager>(TEXT("InventoryManager"));
}

FVoidCoroutine ASessionState::RequestSave(FLatentActionInfo LatentInfo) const
{
	if (SaveObject.IsValid() && ensureAlwaysMsgf(SaveObject->GetCurrentOperation() == EToroSaveOperation::None,
		TEXT("Failed to save global because %s doing an operation."), *GetNameSafe(SaveObject.Get())))
	{
		SaveObject->Session = SessionInfo;
		SaveObject->Flags = SessionFlags->GetFlagList();
		SaveObject->Archives = InventoryManager->ExportArchives();
		SaveObject->Equipment = InventoryManager->GetEquipment().GetTagName();
		co_await SaveObject->SaveToFile(0);
	}
}

FVoidCoroutine ASessionState::RequestLoad(FLatentActionInfo LatentInfo)
{
	if (SaveObject.IsValid() && ensureAlwaysMsgf(SaveObject->GetCurrentOperation() == EToroSaveOperation::None,
		TEXT("Failed to load global because %s doing an operation."), *GetNameSafe(SaveObject.Get())))
	{
		co_await SaveObject->LoadFromFile(0);
		InventoryManager->ImportArchives(SaveObject->Archives);
		SessionFlags->SetFlagList(SaveObject->Flags);
		SessionInfo = SaveObject->Session;
	}
}

void ASessionState::SetGameCompleted()
{
	if (!SessionInfo.IsCompleted())
	{
		SessionInfo.MarkCompletion();
		if (AGlobalState* GlobalState = AGlobalState::Get<AGlobalState>(this))
		{
			GlobalState->AddSessionRecord(SessionInfo);
		}

		RequestSave(FLatentInfo::Make());
	}
}

void ASessionState::BeginPlay()
{
	Super::BeginPlay();
	if (UGameSettings::Get()->IsInMapType(this, EGameMapType::Gameplay))
	{
		UToroSaveManager* SaveManager = UToroSaveManager::Get(this);
		if (ensureAlways(SaveManager))
		{
			SaveObject = SaveManager->GetOrCreateSaveObject<USessionSaveObject>();
			FVoidCoroutine Result = RequestLoad(FLatentInfo::Make(TEXT(""), this));
			Result.ContinueWith([WeakThis = TWeakObjectPtr(this)]()
			{
				if (!WeakThis.IsValid())
				{
					return;
				}

				if (const AToroWorldSettings* WS = AToroWorldSettings::Get(WeakThis.Get()))
				{
					WS->GetGameFlowManager()->StartRootFlow();
					WeakThis->SetActorTickEnabled(true);
				}
			});
		}
	}
}

void ASessionState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!StageManager->IsTransitioning())
	{
		SessionInfo.IncrementPlayTime();
	}
}

void ASessionState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RequestSave(FLatentInfo::Make());
	Super::EndPlay(EndPlayReason);
}
