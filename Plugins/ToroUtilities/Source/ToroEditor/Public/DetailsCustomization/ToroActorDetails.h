// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"

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
	for (const FString& Entry : Entries) Result.Add(*Entry);
	
	return Result.IsEmpty() ? TArray<FName>{*MetaStr} : Result;
}

class TOROEDITOR_API FToroActorDetails : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FToroActorDetails>();
	}

private:

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		TArray<TWeakObjectPtr<UObject>> Objs;
		DetailBuilder.GetObjectsBeingCustomized(Objs);
		
		TArray<FName> Allowed = {TEXT("Actor"), TEXT("Collision"), TEXT("BrushSettings"), TEXT("Tick")};
		TMap<FName, ECategoryPriority::Type> Priority = {
			{TEXT("Transform"), ECategoryPriority::Transform},
			{TEXT("TransformCommon"), ECategoryPriority::Transform},
			{TEXT("Settings"), ECategoryPriority::Important},
			{TEXT("Tools"), ECategoryPriority::Important}
		};
		
		for (const TWeakObjectPtr<UObject>& Obj : Objs)
		{
			if (Obj.IsValid() && !Obj->IsTemplate())
			{
				if (FString Meta = Obj->GetClass()->GetMetaData(TEXT("AllowedCategories")); !Meta.IsEmpty())
				{
					Allowed.Append(GetCleanedArrayMetadata(Meta));
				}

				if (FString Meta = Obj->GetClass()->GetMetaData(TEXT("HiddenCategories")); !Meta.IsEmpty())
				{
					TArray<FName> Temp = GetCleanedArrayMetadata(Meta);
					Allowed.RemoveAll([&Temp](const FName& Category) { return Temp.Contains(Category); });
					for (const FName& Category : Temp)
					{
						Priority.Remove(Category);
					}
				}
			}
		}

		TArray<FName> CategoryNames;
		DetailBuilder.GetCategoryNames(CategoryNames);
		
		for (const TPair<FName, ECategoryPriority::Type>& Pair : Priority)
		{
			if (CategoryNames.Contains(Pair.Key))
			{
				CategoryNames.Remove(Pair.Key);
				DetailBuilder.EditCategory(Pair.Key, Pair.Key == TEXT("TransformCommon")
					? INVTEXT("Transform") : FText::GetEmpty(), Pair.Value);
			}
		}
		
		for (const FName& Name : CategoryNames)
		{
			if (!Allowed.Contains(Name) && !Allowed.ContainsByPredicate([Name](const FName& Elem)
			{
				return Name.ToString().StartsWith(Elem.ToString() + TEXT("|"));
			}))
			{
				DetailBuilder.HideCategory(Name);
			}
		}
	}
};