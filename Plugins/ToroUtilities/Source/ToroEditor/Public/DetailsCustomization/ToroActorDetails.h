// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailsHelpers.h"
#include "ToroVolume.h"

#define GetClassMetadata(MetaKey) GetClass()->GetMetaData(TEXT(#MetaKey))

inline TArray<FName> GetCleanedArrayMetadata(const FString& InMetadata)
{
	if (InMetadata.IsEmpty()) return {};
	if (!InMetadata.Contains(TEXT(","))) return {*InMetadata};

	FString MetaStr = InMetadata.Replace(TEXT(",,"), TEXT(","));
	MetaStr.RemoveSpacesInline();

	TArray<FString> Entries;
	MetaStr.ParseIntoArray(Entries, TEXT(","));

	TArray<FName> Result;
	Result.Reserve(Entries.Num());
	for (const FString& Entry : Entries)
	{
		if (!Entry.Contains(TEXT("|"))) Result.Add(*Entry);
	}

	return Result.IsEmpty() ? TArray<FName>{*MetaStr} : Result;
}

class TOROEDITOR_API FToroActorCustomization : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FToroActorCustomization>();
	}

protected:

	TArray<FName> HiddenCategories;
	TArray<FName> AllowedCategories = { "Transform", "TransformCommon", "Settings", "Tools", "Rendering", "Actor", "Tick" };
	TMap<FName, FText> CategoryRename = {{"TransformCommon", INVTEXT("Transform")}};
	TMap<FName, ECategoryPriority::Type> CategoryPriority = {
		{"Transform",		ECategoryPriority::Transform},
		{"TransformCommon",	ECategoryPriority::Transform},
		{"Settings",			ECategoryPriority::Important},
		{"Tools",			ECategoryPriority::Important},
		{"Rendering",		ECategoryPriority::Default},
		{"Actor",			ECategoryPriority::Default}
	};

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		TArray<TWeakObjectPtr<UObject>> Objs;
		DetailBuilder.GetObjectsBeingCustomized(Objs);
		for (const TWeakObjectPtr<UObject>& Obj : Objs)
		{
			if (!Obj.IsValid()) continue;
			if (Obj->IsTemplate()) return;

			if (Obj->IsA<AToroVolume>()) AllowedCategories.Append({TEXT("Collision"), TEXT("BrushSettings")});
			AllowedCategories.Append(GetCleanedArrayMetadata(Obj->GetClassMetadata(AllowedCategories)));
			HiddenCategories.Append(GetCleanedArrayMetadata(Obj->GetClassMetadata(HiddenCategories)));

			for (const FName& Category : HiddenCategories)
			{
				AllowedCategories.Remove(Category);
				CategoryPriority.Remove(Category);
				CategoryRename.Remove(Category);
			}
		}

		TArray<FName> CategoryNames;
		DetailBuilder.GetCategoryNames(CategoryNames);
		CategoryNames.RemoveAll([](const FName& Element)
		{
			return Element.ToString().Contains(TEXT("|")); // Skip SubCategories
		});

		DetailBuilder.EditCategory("Settings").AddProperty(GET_CLASS_PROPERTY(AToroActor, bEnabled));
		for (const FName& Name : CategoryNames)
		{
			if (!AllowedCategories.Contains(Name))
			{
				DetailBuilder.HideCategory(Name);
			}
			else if (CategoryRename.Contains(Name) || CategoryPriority.Contains(Name))
			{
				DetailBuilder.EditCategory(Name, CategoryRename.FindRef(Name),
					CategoryPriority.Contains(Name) ? CategoryPriority.FindRef(Name) : ECategoryPriority::Default);
			}
		}
	}
};