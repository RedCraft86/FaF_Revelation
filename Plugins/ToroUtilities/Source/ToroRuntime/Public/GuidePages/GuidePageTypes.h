// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "NativeGameplayTags.h"
#include "GuidePageTypes.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_GuidePages);

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FGuidePageEntry
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
	
	FGuidePageEntry()
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
class TORORUNTIME_API UGuidePageDatabase final : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UGuidePageDatabase() {}

	UPROPERTY(EditAnywhere, Category = LocalSound, meta = (ForceInlineRow, Categories = "GuidePages", TitleProperty = "Label"))
		TMap<FGameplayTag, FGuidePageEntry> Entries;

	bool IsKeyValid(const FGameplayTag& Key) const;
	uint8 GetValidCount() const;
	
	static bool IsValidKey(const FGameplayTag& Key);
	static FGuidePageEntry Get(const FGameplayTag& Key);
	
#if WITH_EDITOR
private:
	void UpdateEntries();
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
