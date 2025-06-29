// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NarrativeComponent.h"
#include "DataTypes/InputModeData.h"
#include "ToroNarrative.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TOROSYSTEMS_API UToroNarrative final : public UNarrativeComponent
{
	GENERATED_BODY()

public:

	UToroNarrative() {}

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = "ContextObject"))
		static UToroNarrative* GetNarrative(const UObject* ContextObject, const int32 PlayerIndex);

	static UToroNarrative* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		return GetNarrative(ContextObject, PlayerIndex);
	}

	void ToggleQuests() const;
	virtual void DialogueBegan(UDialogue* Dialogue) override;
	virtual void DialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue) override;

private:

	UPROPERTY() FGameInputModeData CachedInput;
	UPROPERTY(Transient) TObjectPtr<class AToroPlayerCharacter> Player;
	UPROPERTY(Transient) TObjectPtr<class UDialogueWidget> DialogueWidget;

	UDialogueWidget* GetDialogueWidget();
	virtual void BeginPlay() override;
};
