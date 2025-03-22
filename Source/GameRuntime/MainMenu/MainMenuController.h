// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MainMenuWidget.h"
#include "Misc/SequenceActor.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "MainMenuController.generated.h"

UE_DECLARE_GAMEPLAY_TAG_BASE(MainMenu);

UCLASS(NotBlueprintable)
class GAMERUNTIME_API AMainMenuController final : public AActor
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

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ForceInlineRow, Categories = "MainMenu"))
		TMap<FGameplayTag, TSoftObjectPtr<UWorld>> MenuSets;

protected:
	
	void StartLoad() const;
	virtual void BeginPlay() override;
};