// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/ToroDataAsset.h"
#include "Helpers/GameplayTagHelpers.h"
#include "AchievementTypes.generated.h"

DECLARE_GAMEPLAY_TAG_BASE(Achievement)

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FAchievementEntry
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = Achievement, meta = (EditCondition = false, EditConditionHides))
		FString Label = TEXT("");
#endif
	
	UPROPERTY(EditAnywhere, Category = Achievement)
		TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, Category = Achievement)
		FText Name;
	
	UPROPERTY(EditAnywhere, Category = Achievement)
		FText Description;

	UPROPERTY(EditAnywhere, Category = Achievement, meta = (ClampMin = 1, UIMin = 1, ClampMax = 200, UIMax = 200))
		uint8 Requirement;

	FAchievementEntry(): Requirement(1) {}
	bool IsValidData() const { return !Icon.IsNull() && !Name.IsEmptyOrWhitespace(); }
};

UCLASS(NotBlueprintable, BlueprintType)
class TOROUTILITIES_API UAchievementDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:

	UAchievementDatabase() {}
	static bool IsValidKey(const FGameplayTag& Key);
	static FAchievementEntry Get(const FGameplayTag& Key);

	UPROPERTY(EditAnywhere, Category = Achievements)
		TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = Achievements, meta = (ForceInlineRow, Categories = "Achievement", TitleProperty = "Label"))
		TMap<FGameplayTag, FAchievementEntry> Entries;

	bool IsKeyValid(const FGameplayTag& Key) const
	{
		return VerifyAchievementTag(Key) && Entries.Contains(Key) && Entries.FindRef(Key).IsValidData();
	}

	const FAchievementEntry* Find(const FGameplayTag& Key) const
	{
		return IsKeyValid(Key) ? Entries.Find(Key) : nullptr;
	}

#if WITH_EDITOR
	virtual int32 GetValidData() const override;
	virtual int32 GetTotalData() const override { return Entries.Num(); }
protected:
	virtual void UpdateData() override;
#endif
};
