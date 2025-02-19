// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "AchievementTypes.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Achievements);

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FAchievementEntry
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Achievement)
		FString Title;

	FAchievementEntry() {}
};

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UAchievementDatabase final : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UAchievementDatabase() {}

	UPROPERTY(EditAnywhere, Category = LocalSound, meta = (ForceInlineRow, Categories = "Achievements", TitleProperty = "Title"))
		TMap<FGameplayTag, FAchievementEntry> Entries;

	// bool IsKeyValid(const FGameplayTag& Key) const;
	// uint8 GetValidCount() const;
	
	// static bool IsValidKey(const FGameplayTag& Key);
	// static FAchievementEntry Get(const FGameplayTag& Key);
	
#if WITH_EDITOR
private:
	// void UpdateSounds();
	// virtual void PostLoad() override;
	// virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};