// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MainMenuWidget.h"
#include "NativeGameplayTags.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Misc/ToroSequenceActor.h"
#include "MainMenuController.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_MainMenus);

UCLASS(NotBlueprintable)
class FAF_REVELATION_API AMainMenuController final : public AActor
{
	GENERATED_BODY()

public:

	AMainMenuController();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UMainMenuWidgetBase> MainMenuWidget;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<AToroSequenceActor> StartupSequence;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Categories = "MainMenus"))
		TMap<FGameplayTag, TSoftObjectPtr<UWorld>> MenuSets;

protected:

	static inline FGameplayTag GTag_GlobalSave = FGameplayTag::RequestGameplayTag("Saves.Global");
	
	void StartLoad() const;
	virtual void BeginPlay() override;
};
