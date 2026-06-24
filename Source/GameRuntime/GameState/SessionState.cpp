// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "SessionState.h"
#include "GameSettings.h"
#include "GameFlagManager.h"
#include "Helpers/LatentInfo.h"
#include "SaveObjects/SessionSaveObject.h"
#include "SaveSystem/ToroSaveManager.h"

ASessionState::ASessionState()
{
	PrimaryActorTick.bCanEverTick = false;
	SessionFlags = CreateDefaultSubobject<UGameFlagManager>(TEXT("SessionFlags"));
}

FVoidCoroutine ASessionState::RequestSave(FLatentActionInfo LatentInfo) const
{
	if (SaveObject.IsValid() && ensureAlwaysMsgf(SaveObject->GetCurrentOperation() == EToroSaveOperation::None,
		TEXT("Failed to save global because %s doing an operation."), *GetNameSafe(SaveObject.Get())))
	{
		// TODO: Pull data
		SaveObject->Flags = SessionFlags->GetFlagList();
		co_await SaveObject->SaveToFile(0);
	}
}

FVoidCoroutine ASessionState::RequestLoad(FLatentActionInfo LatentInfo) const
{
	if (SaveObject.IsValid() && ensureAlwaysMsgf(SaveObject->GetCurrentOperation() == EToroSaveOperation::None,
		TEXT("Failed to load global because %s doing an operation."), *GetNameSafe(SaveObject.Get())))
	{
		co_await SaveObject->LoadFromFile(0);
		SessionFlags->SetFlagList(SaveObject->Flags);
		// TODO: Push data
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
		}
	}
}

void ASessionState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RequestSave(FLatentInfo::Make());
	Super::EndPlay(EndPlayReason);
}
