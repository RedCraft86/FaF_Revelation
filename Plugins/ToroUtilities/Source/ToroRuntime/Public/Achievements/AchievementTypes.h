// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroDatabase.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "AchievementTypes.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Achievements);

inline bool VerifyAchievementTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_Achievements;
}

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FAchievementEntry
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

	FAchievementEntry() : Requirement(1) {}
	bool IsValidData() const { return !Icon.IsNull() && !Name.IsEmptyOrWhitespace(); }
};

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UAchievementDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:
	
	UAchievementDatabase()
		: NotifyFormat(NSLOCTEXT("Toro", "AchievementNotifyFormat", "[25pt](Achievement Unlocked)\n{0}"))
	{}

	UPROPERTY(EditAnywhere, Category = Achievements, meta = (Multiliine = true))
		FText NotifyFormat;

	UPROPERTY(EditAnywhere, Category = Achievements)
		TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = Achievements, meta = (ForceInlineRow, Categories = "Achievements", TitleProperty = "Label"))
		TMap<FGameplayTag, FAchievementEntry> Entries;

	bool IsKeyValid(const FGameplayTag& Key) const;

	static bool IsValidKey(const FGameplayTag& Key);
	static FAchievementEntry Get(const FGameplayTag& Key);

#if WITH_EDITOR
	virtual int32 GetTotalData() const override;
	virtual int32 GetValidData() const override;
protected:
	virtual void UpdateData() override;
#endif
};