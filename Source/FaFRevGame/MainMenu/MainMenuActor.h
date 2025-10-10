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

	UPROPERTY(EditAnywhere, Category = Settings)
		TSoftObjectPtr<UWorld> DefaultTheme;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ReadOnlyKeys = true))
		TMap<FGameplayTag, TSoftObjectPtr<UWorld>> MenuThemes;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<ALevelSequenceActor> PreThemeChange;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<ALevelSequenceActor> PostThemeChange;

	bool SetMenuTheme(const FGameplayTag& Ending);
	const TArray<FGameplayTag>& GetEndings() const { return AvailableEndings; }

protected:

	bool bFirstLoad;
	FGameplayTag MenuTheme;
	TArray<FGameplayTag> AvailableEndings;

	UFUNCTION() void OnThemeChanged();
	void LoadThemeLevel();

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
