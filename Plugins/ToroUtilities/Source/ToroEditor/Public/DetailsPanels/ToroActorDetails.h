// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"
#include "DetailCategoryBuilder.h"
#include "CustomizationHelpers.h"
#include "Framework/ToroVolume.h"
#include "Framework/ToroCharacter.h"
#include "Helpers/LoggingMacros.h"

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

protected:

	TArray<FName> AllowedCategories;

	virtual TMap<FName, FToroCategoryInfo> GetForcedCategories()
	{
		return {
			{"Transform",		{ECategoryPriority::Transform}},
			{"TransformCommon",	{ECategoryPriority::Transform, INVTEXT("Transform")}},
			{"Tick",				{ECategoryPriority::Important}},
			//{"CharGeneral",		{ECategoryPriority::Important, INVTEXT("General")}},
			{"Settings",			{ECategoryPriority::Important}},
			{"Tools",			{ECategoryPriority::Important}},
			{"Rendering",		{ECategoryPriority::Default}},
			{"Actor",			{ECategoryPriority::Default}}
		};
	}

	virtual void PreHideCategories(IDetailLayoutBuilder& DetailBuilder)
	{
		IDetailCategoryBuilder& Settings = DetailBuilder.EditCategory("Settings");
		TArray<TSharedRef<IPropertyHandle>> Properties = {
			CLASS_PROPERTY(AToroActor, bEnabled),
			CLASS_PROPERTY(AToroCharacter, CharacterID)
		};
		for (TSharedRef<IPropertyHandle>& Property : Properties)
		{
			if(Property->IsValidHandle()) Settings.AddProperty(Property);
		}
	}

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		TMap<FName, FToroCategoryInfo> CategoryInfos = GetForcedCategories();

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
			if (Obj->IsA<AToroVolume>())
			{
				CategoryInfos.Add(TEXT("Collision"));
				CategoryInfos.Add(TEXT("BrushSettings"));
			}
			if (Obj->IsA<AToroCharacter>() && !AllowedCategories.Contains(TEXT("Rendering")))
			{
				CategoryInfos.Remove(TEXT("Rendering"));
			}
		}

		PreHideCategories(DetailBuilder);
		
		TArray<FName> CategoryNames;
		DetailBuilder.GetCategoryNames(CategoryNames);
		CategoryNames.RemoveAll([](const FName& Element)
		{
			return Element.ToString().Contains(TEXT("|")); // Skip SubCategories
		});

		for (const FName& Name : CategoryNames)
		{
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