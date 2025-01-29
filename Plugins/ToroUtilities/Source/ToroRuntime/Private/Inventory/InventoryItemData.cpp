// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryItemData.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_InvMeta, "InventoryMeta");
UE_DEFINE_GAMEPLAY_TAG(Tag_InvNameID, "InventoryMeta.NameID");
UE_DEFINE_GAMEPLAY_TAG(Tag_InvDescID, "InventoryMeta.DescID");
UE_DEFINE_GAMEPLAY_TAG(Tag_InvMeshID, "InventoryMeta.MeshID");
UE_DEFINE_GAMEPLAY_TAG(Tag_InvKeyID, "InventoryMeta.KeyID");

void FInventoryMetadata::Validate()
{
	for (auto It = Metadata.CreateIterator(); It; ++It)
	{
		if (!It->Key.IsValid())
		{
			It.RemoveCurrent();
		}
	}
}

void FInventoryMetadata::Remove(const FGameplayTag& InKey)
{
	if (InKey.IsValid())
	{
		Metadata.Remove(InKey);
		Validate();
	}
}

void FInventoryMetadata::Add(const FGameplayTag& InKey, const FString& InValue)
{
	if (InKey.IsValid())
	{
		Metadata.Add(InKey, InValue);
		Validate();
	}
}

void FInventoryMetadata::ForEach(const TFunctionRef<void(const FGameplayTag& Key, const FString& Value)>& Func) const
{
	const_cast<FInventoryMetadata*>(this)->Validate();
	for (auto It = Metadata.CreateConstIterator(); It; ++It)
	{
		if (It.Key().IsValid()) Func(It.Key(), It.Value());
	}
}

bool FInventoryMetadata::HasMetadata(const TPair<FGameplayTag, FString>& InMetadata, const bool bAnyValue) const
{
	if (!InMetadata.Key.IsValid()) return false;
	return bAnyValue ? Metadata.Contains(InMetadata.Key) : Metadata.FindRef(InMetadata.Key) == InMetadata.Value;
}

bool FInventoryMetadata::HasAnyMetadata(const TMap<FGameplayTag, FString>& InMetadata, const bool bAnyValue) const
{
	if (Metadata.IsEmpty()) return false;
	for (const TPair<FGameplayTag, FString>& Pair : InMetadata)
	{
		if (HasMetadata(Pair, bAnyValue)) return true;
	}

	return false;
}

bool FInventoryMetadata::HasAllMetadata(const TMap<FGameplayTag, FString>& InMetadata, const bool bAnyValue) const
{
	if (Metadata.IsEmpty()) return false;
	for (const TPair<FGameplayTag, FString>& Pair : InMetadata)
	{
		if (!HasMetadata(Pair, bAnyValue)) return false;
	}

	return true;
}

UInventoryItemData::UInventoryItemData() : Priority(1), DisplayName(NSLOCTEXT("Toro", "GenericItemName", "Generic Item"))
	, Description(NSLOCTEXT("Toro", "GenericItemDesc", "This is a generic item!")), StackingMode(EInventoryStackType::UntilMax)
	, StackingValue(5), ItemType(EInventoryItemType::Uncategorized), PreviewZoom({0.5f, 2.0f})
{
}

FText UInventoryItemData::GetDisplayName(const FInventoryMetadata& InMetadata) const
{
	FText TextFmt = DisplayName;
	if (const FString* NameKey = InMetadata.Find(Tag_InvNameID))
	{
		if (const FInvItemAltName* AltNames = GetFirstAttribute<FInvItemAltName>())
		{
			AltNames->GetName(*NameKey, TextFmt);
		}
	}
	return InjectMetadataToText(TextFmt, InMetadata);
}

FText UInventoryItemData::GetDescription(const FInventoryMetadata& InMetadata) const
{
	FText TextFmt = Description;
	if (const FString* NameKey = InMetadata.Find(Tag_InvDescID))
	{
		if (const FInvItemAltDescription* AltDescriptions = GetFirstAttribute<FInvItemAltDescription>())
		{
			AltDescriptions->GetDescription(*NameKey, TextFmt);
		}
	}
	return InjectMetadataToText(TextFmt, InMetadata);
}

FText UInventoryItemData::InjectMetadataToText(const FText& InTextFmt, const FInventoryMetadata& InMetadata)
{
	FFormatNamedArguments MetaArgs;
	InMetadata.ForEach([&MetaArgs](const FGameplayTag& Key, const FString& Value)
	{
		MetaArgs.Add(TEXT("m") + FInventoryMetadata::GetKeyName(Key), FText::FromString(Value));
	});
	return FText::Format(InTextFmt, MetaArgs);
}

#if WITH_EDITOR
void UInventoryItemData::UpdateEditorPreviews()
{
	DisplayNamePreview = GetDisplayName(DefaultMetadata);
	DescriptionPreview = GetDescription(DefaultMetadata);
}

void UInventoryItemData::PostInitProperties()
{
	Super::PostInitProperties();
	UpdateEditorPreviews();
}

void UInventoryItemData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, PreviewZoom))
	{
		if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(FVector2D, X))
		{
			PreviewZoom.X = FMath::Min(PreviewZoom.X, PreviewZoom.Y - 0.1f);
		}
		else
		{
			PreviewZoom.Y = FMath::Max(PreviewZoom.X + 0.1f, PreviewZoom.Y);
		}
	}
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, Attributes))
	{
		TSet<FName> ExistingAttributes;
		ExistingAttributes.Reserve(Attributes.Num());
		for (auto It = Attributes.CreateIterator(); It; ++It)
		{
			if (It->IsValid() && It->GetScriptStruct())
			{
				const FName StructName = It->GetScriptStruct()->GetFName();
				if (ExistingAttributes.Contains(StructName)) It.RemoveCurrent();
				else ExistingAttributes.Add(StructName);
			}
		}
	}
	UpdateEditorPreviews();
}
#endif
