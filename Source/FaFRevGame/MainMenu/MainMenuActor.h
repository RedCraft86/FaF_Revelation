// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "LevelSequenceActor.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "MainMenuActor.generated.h"

UCLASS(HideCategories = (Replication, Networking, Mobile, Input, RayTracing, Cooking), PrioritizeCategories = (Settings))
class FAFREVGAME_API AMainMenuActor final : public AActor
{
	GENERATED_BODY()

public:

	AMainMenuActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, Category = Settings)
		float FadeTime;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSoftObjectPtr<UWorld> DefaultTheme;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ReadOnlyKeys, ForceInlineRow, Categories = "MenuTheme"))
		TMap<FGameplayTag, TSoftObjectPtr<UWorld>> MenuThemes;

	UFUNCTION(BlueprintCallable, Category = MainMenu, meta = (WorldContext = "ContextObject"))
		static void InitializeMainMenu(const UObject* ContextObject);

	bool SetMenuTheme(const FGameplayTag& ThemeTag);
	FGameplayTag GetMenuTheme() const { return MenuTheme; }
	const TArray<FGameplayTag>& GetThemes() const { return AvailableThemes; }

protected:

	FGameplayTag MenuTheme;
	TArray<FGameplayTag> AvailableThemes;

	UFUNCTION() void OnThemeChanged();
	void LoadThemeLevel();

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
