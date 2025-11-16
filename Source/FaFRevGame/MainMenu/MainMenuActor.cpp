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

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
#if WITH_EDITORONLY_DATA
	SceneRoot->bVisualizeComponent = true;
#endif
	SetRootComponent(SceneRoot);
}

void AMainMenuActor::InitializeMainMenu(const UObject* ContextObject)
{
	AActor* Actor = UGameplayStatics::GetActorOfClass(ContextObject, AMainMenuActor::StaticClass());
	if (AMainMenuActor* MenuActor = Actor ? Cast<AMainMenuActor>(Actor) : nullptr)
	{
		UToroShortcutLibrary::SetCameraFade(ContextObject, 1.0f, FLinearColor::Black, true);
		MenuActor->LoadThemeLevel();
	}
}

void AMainMenuActor::MenuMapLoaded(const UObject* ContextObject)
{
	AActor* Actor = UGameplayStatics::GetActorOfClass(ContextObject, AMainMenuActor::StaticClass());
	if (AMainMenuActor* MenuActor = Actor ? Cast<AMainMenuActor>(Actor) : nullptr)
	{
		UToroShortcutLibrary::StartCameraFade(ContextObject, 1.0f, 0.0f, MenuActor->FadeTime);
		if (UMainMenuWidget* Widget = AToroWidgetManager::GetWidget<UMainMenuWidget>(MenuActor))
		{
			Widget->ShowWidget(MenuActor);
		}
	}
}

bool AMainMenuActor::SetMenuTheme(const FGameplayTag& ThemeTag)
{
	if (MenuTheme != ThemeTag)
	{
		MenuTheme = ThemeTag;
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this, &AMainMenuActor::LoadThemeLevel, FadeTime * 0.5f, false);
		UToroShortcutLibrary::StartCameraFade(this, 0.0f, 1.0f, FadeTime * 0.5f);

		return true;
	}
	return false;
}

void AMainMenuActor::LoadThemeLevel()
{
	TSoftObjectPtr<UWorld> Theme = DefaultTheme;
	if (const TSoftObjectPtr<UWorld>* Map = MenuThemes.Find(MenuTheme); Map && !Map->IsNull())
	{
		Theme = *Map;
	}

	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, LastLevel, LatentInfo::Make(), false);
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, Theme, true, false, LatentInfo::Make());

	LastLevel = Theme;
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
#if WITH_EDITORONLY_DATA
		AvailableThemes.Append(ForceAllowThemes.GetGameplayTagArray());
#endif
	}
}

#if WITH_EDITOR
void AMainMenuActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	MenuThemes.Add(MenuThemeTags::TAG_Default.GetTag(), DefaultTheme);
	MenuThemes.FindOrAdd(MenuThemeTags::TAG_Ending.GetTag());
	const TSet<FGameplayTag> AllTags = GameplayTagHelpers::GetAllLeafTags(MenuThemeTags::TAG_MenuTheme.GetTag());
	for (const FGameplayTag& Tag : AllTags)
	{
		if (Tag != MenuThemeTags::TAG_Default.GetTag()
			&& Tag != MenuThemeTags::TAG_Ending.GetTag())
		{
			MenuThemes.FindOrAdd(Tag);
		}
	}
}
#endif