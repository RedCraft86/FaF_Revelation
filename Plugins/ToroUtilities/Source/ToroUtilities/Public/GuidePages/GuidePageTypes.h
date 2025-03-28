﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCore.h"
#include "ToroDataAsset.h"
#include "Engine/DataAsset.h"
#include "NativeGameplayTags.h"
#include "GuidePageTypes.generated.h"

UE_DECLARE_GAMEPLAY_TAG_BASE(GuidePage)

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FGuidePageEntry
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
class TOROUTILITIES_API UGuidePageDatabase final : public UToroDatabase
{
	GENERATED_BODY()
	
public:
	
	UGuidePageDatabase() {}

	UPROPERTY(EditAnywhere, Category = LocalSound, meta = (ForceInlineRow, Categories = "GuidePage", TitleProperty = "Label"))
		TMap<FGameplayTag, FGuidePageEntry> Entries;

	bool IsKeyValid(const FGameplayTag& Key) const;
	const FGuidePageEntry* Find(const FGameplayTag& Key) const;

	static bool IsValidKey(const FGameplayTag& Key);
	static FGuidePageEntry Get(const FGameplayTag& Key);

#if WITH_EDITOR
	virtual int32 GetValidData() const override;
	virtual int32 GetTotalData() const override { return Entries.Num(); }
protected:
	virtual void UpdateData() override;
#endif
};
