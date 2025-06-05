// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"
#include "DetailCategoryBuilder.h"
#include "CustomizationHelpers.h"
#include "Actors/ToroCharacter.h"
#include "Actors/ToroVolume.h"

#define HasClassMetadata(MetaKey) GetClass()->HasMetaData(MetaKey)
#define GetClassMetadata(MetaKey) GetClass()->GetMetaData(MetaKey)

inline TArray<FName> GetMetaValueArray(const FString& InMetadata)
{
	TArray<FName> Result;
	if (InMetadata.IsEmpty()) return Result;

	const FString Cleaned = InMetadata.Replace(TEXT(" "), TEXT(""));
	
	TArray<FString> Entries;
	Cleaned.ParseIntoArray(Entries, TEXT(","), true);

	for (const FString& Entry : Entries)
	{
		if (!Entry.Contains(TEXT("|")))
			Result.Add(*Entry);
	}

	return Result.IsEmpty() ? TArray<FName>{*Cleaned} : Result;
}

struct FToroCategoryInfo
{
	FText Name;
	TEnumAsByte<ECategoryPriority::Type> Priority;
	
	FToroCategoryInfo(): Priority(ECategoryPriority::Default) {}
	FToroCategoryInfo(const ECategoryPriority::Type InPriority, const FText& InName = FText::GetEmpty())
		: Name(InName), Priority(InPriority)
	{}
};

class TOROEDITOR_API FToroActorDetails : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FToroActorDetails>();
	}

	static inline FName Meta_CustomizeTemplate = TEXT("CustomizeTemplate");
	static inline FName Meta_AllowedCategories = TEXT("AllowedCategories");
	static inline FName Meta_HiddenCategories = TEXT("HiddenCategories");

protected:

	TMap<FName, FToroCategoryInfo> CategoryInfos;
	TArray<FName> AllowedCategories;
	TArray<FName> HiddenCategories;

	void MovePropertiesToCategory(IDetailLayoutBuilder& DetailBuilder, const FName& InCategory,
		TArray<TSharedRef<IPropertyHandle>>& InProperties) const
	{
		const FToroCategoryInfo& Info = CategoryInfos.FindRef(InCategory);
		IDetailCategoryBuilder& Settings = DetailBuilder.EditCategory(InCategory, Info.Name, Info.Priority);
		for (TSharedRef<IPropertyHandle>& Property : InProperties)
		{
			if(Property->IsValidHandle()) Settings.AddProperty(Property);
		}
	}

	virtual TMap<FName, FToroCategoryInfo> GetForcedCategories()
	{
		return {
			{"Transform",		{ECategoryPriority::Transform}},
			{"TransformCommon",{ECategoryPriority::Transform, INVTEXT("Transform")}},
			{"Tick",			{ECategoryPriority::Important}},
			{"Settings",		{ECategoryPriority::Important}},
			{"Tools",			{ECategoryPriority::Important}},
			{"Rendering",		{ECategoryPriority::Default}},
			{"Actor",			{ECategoryPriority::Default}}
		};
	}

	virtual void AdjustProperties(IDetailLayoutBuilder& DetailBuilder)
	{
		TArray<TSharedRef<IPropertyHandle>> SettingsProperties = {
			CLASS_PROPERTY(AToroActor, bEnabled),
			CLASS_PROPERTY(AToroCharacter, CharacterID)
		};
		MovePropertiesToCategory(DetailBuilder, "Settings", SettingsProperties);
	}

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		CategoryInfos = GetForcedCategories();

		const UClass* ClassType = nullptr;
		TArray<TWeakObjectPtr<UObject>> Objs;
		DetailBuilder.GetObjectsBeingCustomized(Objs);
		for (const TWeakObjectPtr<UObject>& Obj : Objs)
		{
			if (!Obj.IsValid()) continue;
			if (Obj->IsTemplate() && !Obj->HasClassMetadata(Meta_CustomizeTemplate)) continue;
			
			// We usually don't want to mix different class types
			if (!ClassType) ClassType = Obj->GetClass();
			else if (ClassType != Obj->GetClass()) return;
			
			AllowedCategories.Append(GetMetaValueArray(Obj->GetClassMetadata(Meta_AllowedCategories)));
			HiddenCategories.Append(GetMetaValueArray(Obj->GetClassMetadata(Meta_HiddenCategories)));
			if (Obj->IsA<AToroVolume>())
			{
				CategoryInfos.Add(TEXT("Collision"), {ECategoryPriority::Uncommon});
				CategoryInfos.Add(TEXT("BrushSettings"), {ECategoryPriority::Uncommon});
			}
			if (Obj->IsA<AToroCharacter>() && !AllowedCategories.Contains(TEXT("Rendering")))
			{
				CategoryInfos.Remove(TEXT("Rendering"));
			}
		}

		AdjustProperties(DetailBuilder);
		
		TArray<FName> CategoryNames;
		DetailBuilder.GetCategoryNames(CategoryNames);
		CategoryNames.RemoveAll([](const FName& Element)
		{
			return Element.ToString().Contains(TEXT("|")); // Skip SubCategories
		});

		for (const FName& Name : CategoryNames)
		{
			if (HiddenCategories.Contains(Name))
			{
				DetailBuilder.HideCategory(Name);
				continue;
			}
			
			if (CategoryInfos.Contains(Name))
			{
				const FToroCategoryInfo& Info = CategoryInfos[Name];
				DetailBuilder.EditCategory(Name, Info.Name, Info.Priority);
			}
			else if (!AllowedCategories.Contains(Name))
			{
				DetailBuilder.HideCategory(Name);
			}
		}
	}
};