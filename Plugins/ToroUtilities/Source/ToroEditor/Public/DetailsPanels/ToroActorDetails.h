// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CustomizationHelpers.h"
#include "DetailCategoryBuilder.h"
#include "IDetailCustomization.h"
#include "DetailLayoutBuilder.h"
#include "ToroEditor.h"

#include "Actors/ToroActor.h"
#include "Actors/ToroVolume.h"
#include "Actors/ToroCharacter.h"

class TOROEDITOR_API FToroActorDetails : public IDetailCustomization
{
public:

	static inline FName Meta_CustomizeTemplate	= TEXT("CustomizeTemplate");
	static inline FName Meta_VisibleCategories	= TEXT("VisibleCategories");
	static inline FName Meta_HiddenCategories	= TEXT("HiddenCategories");

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FToroActorDetails>();
	}

protected:

	struct FCategoryInfo
	{
		TEnumAsByte<ECategoryPriority::Type> Priority;
		FText DisplayName;

		FCategoryInfo(): Priority(ECategoryPriority::Default) {}
		FCategoryInfo(const ECategoryPriority::Type InPriority, const FText& InName = FText::GetEmpty())
			: Priority(InPriority), DisplayName(InName)
		{}
	};

	TMap<FName, FCategoryInfo> CachedCategories;
	TArray<FName> VisibleCategories;
	TArray<FName> HiddenCategories;

	virtual TMap<FName, FCategoryInfo> GetNativeCategories()
	{
		return {
			{"Transform",			{ECategoryPriority::Transform}},
			{"TransformCommon",	{ECategoryPriority::Transform, INVTEXT("Transform")}},
			{"Tick",				{ECategoryPriority::Important}},
			{"Settings",			{ECategoryPriority::Important}},
			{"Tools",				{ECategoryPriority::Important}},
			{"Rendering",			{ECategoryPriority::Default}},
			{"Actor",				{ECategoryPriority::Default}}
		};
	}

	virtual void AdjustProperties(IDetailLayoutBuilder& DetailBuilder)
	{
		MoveProperties(DetailBuilder, "Settings", {
			CLASS_PROPERTY(AToroActor, bEnabled),
			CLASS_PROPERTY(AToroCharacter, CharacterID)
		});
	}

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		CachedCategories = GetNativeCategories();

		const UClass* Class = nullptr;
		TArray<TWeakObjectPtr<UObject>> Objects;
		DetailBuilder.GetObjectsBeingCustomized(Objects);
		for (const TWeakObjectPtr<UObject>& Object : Objects)
		{
			if (!Object.IsValid()) continue;

			if (!Class) Class = Object->GetClass();
			else if (Class != Object->GetClass()) return;

			if (Object->IsTemplate() && !Class->HasMetaData(Meta_CustomizeTemplate))
			{
				continue;
			}

			VisibleCategories.Append(ExtractCategories(Class->GetMetaData(Meta_VisibleCategories)));
			HiddenCategories.Append(ExtractCategories(Class->GetMetaData(Meta_HiddenCategories)));

			if (Object->IsA<AToroVolume>())
			{
				CachedCategories.Add("Collision", {ECategoryPriority::Uncommon});
				CachedCategories.Add("BrushSettings", {ECategoryPriority::Uncommon});
			}
			else if (Object->IsA<AToroCharacter>() && !VisibleCategories.Contains("Rendering"))
			{
				CachedCategories.Remove("Rendering");
			}
		}

		AdjustProperties(DetailBuilder);

		TArray<FName> AllCategories;
		DetailBuilder.GetCategoryNames(AllCategories);
		AllCategories.RemoveAll([](const FName& Element)
		{
			return Element.ToString().Contains(TEXT("|")); // Skip SubCategories
		});

		for (const FName& Category : AllCategories)
		{
			// All hidden unless specified by VisibleCategories or force loaded in CachedCategories
			// HiddenCategories applies to categories that have been force loaded
			if (HiddenCategories.Contains(Category))
			{
				DetailBuilder.HideCategory(Category);
			}
			else if (CachedCategories.Contains(Category))
			{
				const FCategoryInfo& Info = CachedCategories[Category];
				DetailBuilder.EditCategory(Category, Info.DisplayName, Info.Priority);
			}
			else if (!VisibleCategories.Contains(Category))
			{
				DetailBuilder.HideCategory(Category);
			}
		}
	}

	void MoveProperties(IDetailLayoutBuilder& Builder, const FName& ToCategory,
		const TArray<TSharedRef<IPropertyHandle>>& InProperties)
	{
		const FCategoryInfo& Info = CachedCategories.FindOrAdd(ToCategory);
		IDetailCategoryBuilder& Settings = Builder.EditCategory(ToCategory, Info.DisplayName, Info.Priority);
		for (const TSharedRef<IPropertyHandle>& Property : InProperties)
		{
			if(Property->IsValidHandle()) Settings.AddProperty(Property);
		}
	}

	static TArray<FName> ExtractCategories(const FString& InMetaValue)
	{
		TArray<FName> Result;
		if (InMetaValue.IsEmpty())
		{
			return Result;
		}

		const FString Cleaned = InMetaValue.Replace(TEXT(" "), TEXT(""));

		TArray<FString> Entries;
		Cleaned.ParseIntoArray(Entries, TEXT(","), true);
		if (Entries.IsEmpty())
		{
			if (!Cleaned.IsEmpty())
			{
				Result.Add(*Cleaned);
			}
			return Result;
		}

		for (const FString& Entry : Entries)
		{
			if (!Entry.Contains(TEXT("|")))
			{
				Result.Add(*Entry);
			}
			else
			{
				UE_LOG(LogToroEditor, Warning,
					TEXT("[ToroActorDetails] Subcategories are not supported in FToroActorDetails.\n%s"),
					*InMetaValue
				);
			}
		}

		return Result;
	}
};
