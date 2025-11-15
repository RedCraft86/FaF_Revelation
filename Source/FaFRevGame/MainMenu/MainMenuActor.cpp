// Copyright (C) RedCraft86. All Rights Reserved.

#include "MainMenuActor.h"
#include "MainMenuWidget.h"
#include "Helpers/LatentInfo.h"
#include "SaveSystem/ToroGlobalSave.h"
#include "SaveSystem/ToroSaveManager.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "UserInterface/ToroWidgetManager.h"
#include "GamePhase/GamePhaseManager.h"

AMainMenuActor::AMainMenuActor(): FadeTime(2.0f)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

void AMainMenuActor::InitiateMainMenu(const UObject* ContextObject)
{
	AActor* Actor = UGameplayStatics::GetActorOfClass(ContextObject, AMainMenuActor::StaticClass());
	if (AMainMenuActor* MenuActor = Actor ? Cast<AMainMenuActor>(Actor) : nullptr)
	{
		UToroShortcutLibrary::SetCameraFade(ContextObject, 1.0f, FLinearColor::Black, true);
		MenuActor->LoadThemeLevel();
	}
}

bool AMainMenuActor::SetMenuTheme(const FGameplayTag& ThemeTag)
{
	if (MenuTheme != ThemeTag)
	{
		MenuTheme = ThemeTag;
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this, &AMainMenuActor::LoadThemeLevel, FadeTime, false);
		UToroShortcutLibrary::StartCameraFade(this, 0.0f, 1.0f, FadeTime);

		return true;
	}
	return false;
}

void AMainMenuActor::OnThemeChanged()
{
	UToroShortcutLibrary::StartCameraFade(this, 1.0f, 0.0f, FadeTime);
	if (UMainMenuWidget* Widget = AToroWidgetManager::GetWidget<UMainMenuWidget>(this))
	{
		Widget->ShowWidget(this);
	}
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
	if (const UToroGlobalSave* GlobalSave = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>(0) : nullptr)
	{
		AvailableThemes = GlobalSave->Themes;
		if (!AvailableThemes.IsEmpty())
		{
			MenuTheme = AvailableThemes.Last();
		}
	}
}

#if WITH_EDITOR
void AMainMenuActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	const FGameplayTagContainer AllThemes = UGameplayTagsManager::Get()
		.RequestGameplayTagChildren(MenuThemeTags::TAG_MenuTheme.GetTag());

	for (const FGameplayTag& Tag : AllThemes)
	{
		MenuThemes.FindOrAdd(Tag);
	}
}
#endif