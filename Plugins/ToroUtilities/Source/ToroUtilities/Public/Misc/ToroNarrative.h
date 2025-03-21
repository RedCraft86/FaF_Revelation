// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NarrativeComponent.h"
#include "Framework/ToroPlayerController.h"
#include "ToroNarrative.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TOROUTILITIES_API UToroNarrativeComponent : public UNarrativeComponent
{
	GENERATED_BODY()

public:

	UToroNarrativeComponent() {}
	static UToroNarrativeComponent* Get(const UObject* WorldContext);

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UUserWidget> WidgetClass;

	virtual void DialogueBegan(UDialogue* Dialogue) override;
	virtual void DialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue) override;

protected:

	UPROPERTY() FGameInputModeData CachedInputMode;
	UPROPERTY(Transient) TObjectPtr<class AToroPlayerCharacter> Player;

	virtual UUserWidget* GetWidget() const;
	virtual void BeginPlay() override;
};
