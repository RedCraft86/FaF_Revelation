// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenuController.h"
#include "Algo/RandomShuffle.h"
#include "Characters/ToroPlayerBase.h"
#include "Framework/ToroWidgetManager.h"
#include "SaveSystem/BaseSaveObjects.h"
#include "SaveSystem/ToroSaveManager.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_MainMenus, "MainMenus")

AMainMenuController::AMainMenuController()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
#if WITH_EDITORONLY_DATA
	Root->bVisualizeComponent = true;
#endif
}

void AMainMenuController::StartLoad() const
{
	TArray<TSoftObjectPtr<UWorld>> Scenes;
	MenuSets.GenerateValueArray(Scenes);
	Algo::RandomShuffle(Scenes);
	if (!Scenes.IsEmpty())
	{
		FLatentActionInfo Info;
		Info.ExecutionFunction = NAME_None;
		Info.CallbackTarget = nullptr;
		Info.Linkage = 0;
		Info.UUID = 0;

		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, Scenes[0],
			true, false, Info);
	}

	if (const AToroPlayerBase* Player = AToroPlayerBase::Get(this))
	{
		Player->FadeFromBlack(2.0f, false);
	}

	if (AToroWidgetManager* WidgetManager = AToroWidgetManager::Get(this))
	{
		WidgetManager->FindOrAddWidget(MainMenuWidget);
	}
}

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();
	const UToroGameInstance* GI = UToroGameInstance::Get(this);
	if (GI && GI->FinishedFirstLoads())
	{
		if (AToroPlayerBase* Player = AToroPlayerBase::Get(this))
		{
			Player->AddLockFlag(FGameplayTag::RequestGameplayTag("PlayerLock.MainMenu"));
			Player->TeleportPlayer(FVector::ZeroVector, FRotator::ZeroRotator);
		}

		StartupSequence->Play();
		StartupSequence->OnSequenceFinished().BindUObject(this, &ThisClass::StartLoad);
	}

	if (UGlobalSaveObjectBase* Save = ToroSave::GetObject<UGlobalSaveObjectBase>(this, GTag_GlobalSave))
	{
		for (auto It = MenuSets.CreateIterator(); It; ++It)
		{
			if (!Save->Content.Contains(It->Key) && It->Key != Tag_MainMenus)
			{
				It.RemoveCurrent();
			}
		}
	}
}
