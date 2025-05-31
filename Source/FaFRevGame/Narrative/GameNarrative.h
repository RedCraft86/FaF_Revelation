// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NarrativeComponent.h"
#include "DataTypes/InputModeData.h"
#include "GameNarrative.generated.h"

UCLASS()
class FAFREVGAME_API UGameNarrative final : public UNarrativeComponent
{
	GENERATED_BODY()

public:

	UGameNarrative() {}

	static UGameNarrative* Get(const UObject* ContextObject);
	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = "ContextObject"))
		static UGameNarrative* GetNarrative(const UObject* ContextObject) { return Get(ContextObject); }

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UUserWidget> NarrativeWidget; // TODO: Correct type?

	virtual void DialogueBegan(UDialogue* Dialogue) override;
	virtual void DialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue) override;

protected:

	UPROPERTY() FGameInputModeData CachedInputMode;
	UPROPERTY(Transient) TObjectPtr<class AGamePlayer> Player;

	UUserWidget* GetWidget() const;
	virtual void BeginPlay() override;
};
