// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Tutorials/TutorialManager.h"
#include "SaveSystem/ToroGlobalSave.h"
#include "ToroRuntime.h"

void UTutorialManager::QueueTutorial(const FGameplayTag Key)
{
	const UToroGlobalSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>() : nullptr;
	if (!Save || !Save->Tutorials.Contains(Key))
	{
		QueueInternal(Key);
	}
}

void UTutorialManager::QueueTutorials(const TArray<FGameplayTag>& Keys)
{
	const UToroGlobalSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>() : nullptr;
	for (const FGameplayTag& Key : Keys)
	{
		if (!Save || !Save->Tutorials.Contains(Key))
		{
			QueueInternal(Key);
		}
	}
}

void UTutorialManager::MarkTutorialsSeen()
{
	if (UToroGlobalSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>() : nullptr)
	{
		Save->Tutorials.Append(Tutorials);
		Save->SaveObject(nullptr);
	}
	Tutorials.Empty();
}

const FTutorialEntry* UTutorialManager::GetTutorialData(const FGameplayTag& Key) const
{
	return Database ? Database->GetEntry(Key) : nullptr;
}

void UTutorialManager::QueueInternal(const FGameplayTag& Key)
{
	if (TutorialTags::IsValidTag(Key) && !Tutorials.Contains(Key))
	{
		Tutorials.Add(Key);
		// TODO ui
	}
}

void UTutorialManager::BeginPlay()
{
	Super::BeginPlay();
	SaveManager = UToroSaveManager::Get(this);
	Database = UToroSettings::Get()->GetDatabase<UTutorialDatabase>();
#if WITH_EDITOR
	if (!Database)
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Tutorial Database not found in settings!"))
	}
#endif
}
