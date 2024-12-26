// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"

inline TArray<FString> GetCleanedArrayMetadata(const FString& InMetadata)
{
	if (InMetadata.IsEmpty()) return {};
	if (!InMetadata.Contains(TEXT(","))) return {InMetadata};
	
	FString MetaStr = InMetadata.Replace(TEXT(", "), TEXT(",")).Replace(TEXT(" ,"), TEXT(","));
	while (MetaStr.Contains(TEXT(",,"))) MetaStr.ReplaceInline(TEXT(",,"), TEXT(","));

	TArray<FString> Result;
	MetaStr.ParseIntoArray(Result, TEXT(","));
	return Result.IsEmpty() ? TArray<FString>{MetaStr} : Result;
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
		
		TArray<FString> Allowed = {TEXT("Transform"), TEXT("TransformCommon"), TEXT("Settings"),
			TEXT("Actor"), TEXT("Collision"), TEXT("BrushSettings"), TEXT("Tick")};
		
		for (TWeakObjectPtr<UObject> Obj : Objs)
		{
			if (!Obj.IsValid() || Obj->IsTemplate()) continue;
			if (FString Meta = Obj->GetClass()->GetMetaData(TEXT("AllowedCategories")); !Meta.IsEmpty())
			{
				Allowed.Append(GetCleanedArrayMetadata(Meta));
			}

			if (FString Meta = Obj->GetClass()->GetMetaData(TEXT("HiddenCategories")); !Meta.IsEmpty())
			{
				TArray<FString> Temp = GetCleanedArrayMetadata(Meta);
				Allowed.RemoveAll([Temp](const FString& Element) -> bool { return Temp.Contains(Element); });
			}
		}

		TArray<FName> CategoryNames;
		DetailBuilder.GetCategoryNames(CategoryNames);
		for (const FName& Name : CategoryNames)
		{
			if (!Allowed.ContainsByPredicate([Name](const FString& Elem) -> bool
			{
				return Name.ToString().Equals(Elem) || Name.ToString().StartsWith(Elem + TEXT("|"));
			}))
			{
				DetailBuilder.HideCategory(Name);
			}
		}
	}
};