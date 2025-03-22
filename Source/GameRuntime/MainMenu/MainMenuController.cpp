// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenuController.h"
#include "SaveSystem/ToroSaveManager.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroWidgetManager.h"
#include "Framework/ToroGameInstance.h"
#include "Algo/RandomShuffle.h"
#include "GameSaveObjects.h"
#include "LoggingHelpers.h"

UE_DEFINE_GAMEPLAY_TAG_BASE(MainMenu)

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
	if (const AToroPlayerCharacter* Player = AToroPlayerCharacter::Get(this))
	{
		Player->FadeFromBlack(2.0f, false);
	}
	if (UMainMenuWidgetBase* Widget = CreateWidget<UMainMenuWidgetBase>(GetWorld(), MainMenuWidget))
	{
		Widget->AddToViewport(50);
	}
}

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();
	const UToroGameInstance* GI = UToroGameInstance::Get(this);
	if (GI && GI->FinishedFirstLoads())
	{
		if (AToroPlayerCharacter* Player = AToroPlayerCharacter::Get(this))
		{
			Player->AddLockFlag(GAMEPLAY_TAG_CHILD(MainMenu, PlayerLock));
			Player->Teleport(FVector::ZeroVector, FRotator::ZeroRotator);
		}

		if (StartupSequence)
		{
			StartupSequence->Play();
			StartupSequence->OnSequenceFinished().BindUObject(this, &ThisClass::StartLoad);
		}
		else StartLoad();
	}

	if (const UGlobalSaveObject* Save = ToroSave::GetObject<UGlobalSaveObject>(this, GAMEPLAY_TAG_CHILD(Global, Save)))
	{
		for (auto It = MenuSets.CreateIterator(); It; ++It)
		{
			if (!Save->Content.Contains(It->Key) && It->Key != Tag_MainMenu)
			{
				It.RemoveCurrent();
			}
		}
	}
}