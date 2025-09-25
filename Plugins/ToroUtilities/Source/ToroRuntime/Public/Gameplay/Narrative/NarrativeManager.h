// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NarrativeComponent.h"
#include "DataTypes/GameInputConfig.h"
#include "Helpers/ClassGetterMacros.h"
#include "Framework/ToroPlayerState.h"
#include "NarrativeManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Player), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UNarrativeManager : public UNarrativeComponent
{
	GENERATED_BODY()

public:

	UNarrativeManager() {}
	
	PLAYER_COMPONENT_GETTER(UNarrativeManager, AToroPlayerState, Narrative)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UNarrativeManager* GetNarrativeManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}

	void ToggleQuests() const;
	virtual void DialogueBegan(UDialogue* Dialogue) override;
	virtual void DialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue) override;

private:

	FGameInputConfig CachedInputConfig;
};
