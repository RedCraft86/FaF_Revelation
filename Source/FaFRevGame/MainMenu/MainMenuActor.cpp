// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenuActor.h"
#include "MainMenuWidget.h"
#include "Helpers/LatentInfo.h"
#include "SaveSystem/ToroGlobalSave.h"
#include "SaveSystem/ToroSaveManager.h"
#include "UserInterface/ToroWidgetManager.h"
#include "GamePhase/GamePhaseManager.h"
#include "LevelSequencePlayer.h"

AMainMenuActor::AMainMenuActor(): bFirstLoad(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

bool AMainMenuActor::SetMenuTheme(const FGameplayTag& Ending)
{
	if (MenuTheme != Ending)
	{
		MenuTheme = Ending;
		if (PreThemeChange && PreThemeChange->GetSequencePlayer())
		{
			PreThemeChange->GetSequencePlayer()->RewindForReplay();
			PreThemeChange->GetSequencePlayer()->Play();
		}
		else
		{
			LoadThemeLevel();
		}
		return true;
	}
	return false;
}

void AMainMenuActor::OnThemeChanged()
{
	if (UMainMenuWidget* Widget = AToroWidgetManager::GetWidget<UMainMenuWidget>(this))
	{
		Widget->ShowWidget(this);
	}
	if (!bFirstLoad && PostThemeChange && PostThemeChange->GetSequencePlayer())
	{
		PostThemeChange->GetSequencePlayer()->RewindForReplay();
		PostThemeChange->GetSequencePlayer()->Play();
	}
	bFirstLoad = false;
}

void AMainMenuActor::LoadThemeLevel()
{
	TSoftObjectPtr<UWorld> Theme = DefaultTheme;
	if (MenuThemes.Contains(MenuTheme))
	{
		Theme = MenuThemes[MenuTheme];
	}

	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, Theme, true,
		false, LatentInfo::Make(TEXT("OnThemeChanged"), this));
}

void AMainMenuActor::BeginPlay()
{
	Super::BeginPlay();
	UToroSaveManager* SaveManager = UToroSaveManager::Get(this);
	if (UToroGlobalSave* GlobalSave = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>(0) : nullptr)
	{
		GlobalSave->Endings.GenerateKeyArray(AvailableEndings);
		if (AvailableEndings.Num() > 1)
		{
			AvailableEndings.Sort([this, GlobalSave](const FGameplayTag& A, const FGameplayTag& B)
			{
				return GlobalSave->Endings[A] < GlobalSave->Endings[B];
			});

			MenuTheme = AvailableEndings.Last();
		}
	}

	LoadThemeLevel();
	if (PreThemeChange && PreThemeChange->GetSequencePlayer())
	{
		PreThemeChange->GetSequencePlayer()->OnNativeFinished
			.BindUObject(this, &AMainMenuActor::LoadThemeLevel);
	}
}

#if WITH_EDITOR
void AMainMenuActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	const FGameplayTagContainer AllEndings = UGameplayTagsManager::Get()
		.RequestGameplayTagChildren(EndingTags::TAG_Ending.GetTag());

	for (const FGameplayTag& Tag : AllEndings)
	{
		MenuThemes.FindOrAdd(Tag);
	}
}
#endif