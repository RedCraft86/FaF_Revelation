// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MiscObjects/ToroDataAsset.h"
#include "Helpers/GameplayTagMacros.h"
#include "TutorialDatabase.generated.h"

namespace TutorialTags
{
	TOROSYSTEMS_API DECLARE_GAMEPLAY_TAG(Tutorial)
	TOROSYSTEMS_API CREATE_TAG_VERIFIER(Tutorial)
}

USTRUCT(BlueprintInternalUseOnly)
struct TOROSYSTEMS_API FTutorialEntry
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = Achievement, meta = (EditCondition = false, EditConditionHides))
		FString Label = TEXT("");
#endif
	
	UPROPERTY(EditAnywhere, Category = GuidePage)
		TSoftClassPtr<UUserWidget> CustomWidget;

	UPROPERTY(EditAnywhere, Category = GuidePage, meta = (EditCondition = "CustomWidget == nullptr"))
		FText Title;

	UPROPERTY(EditAnywhere, Category = GuidePage, meta = (EditCondition = "CustomWidget == nullptr", MultiLine = true))
		FText Description;

	UPROPERTY(EditAnywhere, Category = GuidePage, meta = (EditCondition = "CustomWidget == nullptr"))
		TSoftObjectPtr<UTexture2D> Image;
	
	FTutorialEntry()
		: CustomWidget(nullptr)
		, Title(FText::GetEmpty())
		, Description(FText::GetEmpty())
		, Image(nullptr)
	{}

	FORCEINLINE bool IsValidData() const
	{
		return !CustomWidget.IsNull() || (!Title.IsEmptyOrWhitespace() && !Description.IsEmptyOrWhitespace());
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class TOROSYSTEMS_API UTutorialDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:

	UTutorialDatabase() {}

	UPROPERTY(EditAnywhere, Category = LocalSound, meta = (ForceInlineRow, Categories = "Tutorial", TitleProperty = "Label"))
		TMap<FGameplayTag, FTutorialEntry> Entries;

	static bool IsValidKey(const FGameplayTag& Key);
	static FTutorialEntry Get(const FGameplayTag& Key);

#if WITH_EDITOR
	virtual int32 GetValidData() const override;
	virtual int32 GetTotalData() const override { return Entries.Num(); }
protected:
	virtual void UpdateData() override;
#endif
};
