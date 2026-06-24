// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GlobalState.h"
#include "Helpers/LatentInfo.h"
#include "SaveObjects/GlobalSaveObject.h"
#include "SaveSystem/ToroSaveManager.h"

FVoidCoroutine AGlobalState::RequestSave(FLatentActionInfo LatentInfo) const
{
	if (SaveObject.IsValid() && ensureAlwaysMsgf(SaveObject->GetCurrentOperation() == EToroSaveOperation::None,
		TEXT("Failed to save global because %s doing an operation."), *GetNameSafe(SaveObject.Get())))
	{
		// TODO: Pull data
		co_await SaveObject->SaveToFile(0);
	}
}

FVoidCoroutine AGlobalState::RequestLoad(FLatentActionInfo LatentInfo) const
{
	if (SaveObject.IsValid() && ensureAlwaysMsgf(SaveObject->GetCurrentOperation() == EToroSaveOperation::None,
		TEXT("Failed to load global because %s doing an operation."), *GetNameSafe(SaveObject.Get())))
	{
		co_await SaveObject->LoadFromFile(0);
		// TODO: Push data
	}
}

void AGlobalState::BeginPlay()
{
	Super::BeginPlay();
	UToroSaveManager* SaveManager = UToroSaveManager::Get(this);
	if (ensureAlways(SaveManager))
	{
		SaveObject = SaveManager->GetOrCreateSaveObject<UGlobalSaveObject>();
	}
}

void AGlobalState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RequestSave(FLatentInfo::Make());
	Super::EndPlay(EndPlayReason);
}
