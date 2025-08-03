// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/GameplayTagMacros.h"
#include "MiscObjects/ToroDataAsset.h"
#include "UserSettings/ToroUserSettings.h"
#include "UserSettings/UserSettingTypes.h"
#include "HintDatabase.generated.h"

namespace HintTags
{
	TOROSYSTEMS_API DECLARE_GAMEPLAY_TAG(Hint)
	TOROSYSTEMS_API CREATE_TAG_VERIFIER(Hint)
}

USTRUCT(BlueprintInternalUseOnly)
struct TOROSYSTEMS_API FHintEntry
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = Achievement, meta = (EditCondition = false, EditConditionHides))
		FString Label = TEXT("");
#endif

	UPROPERTY(EditAnywhere, Category = HintEntry, meta = (ClampMin = 1.0f, UIMin = 1.0f))
		float EasyDelay;

	UPROPERTY(EditAnywhere, Category = HintEntry, meta = (ClampMin = 1.0f, UIMin = 1.0f))
		float NormalDelay;

	UPROPERTY(EditAnywhere, Category = HintEntry, meta = (ClampMin = -1.0f, UIMin = -1.0f))
		float HardDelay;

	UPROPERTY(EditAnywhere, Category = HintEntry)
		bool bUseSubtitle;

	UPROPERTY(EditAnywhere, Category = HintEntry, meta = (MultiLine = true))
		FText Message;
	
	FHintEntry(): EasyDelay(60.0f), NormalDelay(120.0f), HardDelay(-1.0f), bUseSubtitle(false) {}
	
	FORCEINLINE bool IsValidData() const { return Message.IsEmptyOrWhitespace() && GetTime() > 0.0f; }
	FORCEINLINE float GetTime() const
	{
#if WITH_EDITOR
		switch (FApp::IsGame() ? UToroUserSettings::Get()->GetDifficulty() : EGameDifficulty::Normal)
#else
		switch (UToroUserSettings::Get()->GetDifficulty())
#endif
		{
		case EGameDifficulty::Easy: return EasyDelay;
		case EGameDifficulty::Normal: return NormalDelay;
		case EGameDifficulty::Hard: return HardDelay;
		}
		return 1.0f;
	}
};

UCLASS()
class TOROSYSTEMS_API UHintDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:

	UHintDatabase() {}

	UPROPERTY(EditAnywhere, Category = LocalSound, meta = (ForceInlineRow, Categories = "Hint", TitleProperty = "Label"))
		TMap<FGameplayTag, FHintEntry> Entries;

	static bool IsValidKey(const FGameplayTag& Key);
	static FHintEntry Get(const FGameplayTag& Key);

#if WITH_EDITOR
	virtual int32 GetValidData() const override;
	virtual int32 GetTotalData() const override { return Entries.Num(); }
protected:
	virtual void UpdateData() override;
#endif
};
