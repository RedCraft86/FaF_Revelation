// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryItemData.h"

UInventoryItemData::UInventoryItemData() : Priority(1), UniqueID(FGuid::NewGuid())
	, DisplayName(INVTEXT("Generic Item")), Description(INVTEXT("This is a generic item!"))
	, StackingMode(EInventoryStackType::UntilMax), StackingValue(5)
	, ItemType(EInventoryItemType::Uncategorized), PreviewZoom({0.5f, 2.0f})
{
#if WITH_EDITORONLY_DATA
	MetadataKeyGuide = {
		{InventoryNativeMeta::NameID,	TEXT("Used with the Alt Names Attribute.")},
		{InventoryNativeMeta::DescID,	TEXT("Used with the Alt Descriptions Attribute.")},
		{InventoryNativeMeta::MeshID,	TEXT("Used with the Alt Meshes Attribute.")},
		{InventoryNativeMeta::KeyID,	TEXT("This item will be able to unlock locks with matching IDs.")}
	};
#endif
}

FText InjectMetadataToText(const FText& InTextFmt, const TMap<FName, FString>& InMetadata)
{
	FFormatNamedArguments MetaArgs;
	for (const TPair<FName, FString>& Meta : InMetadata)
	{
		if (!Meta.Key.IsNone() && !Meta.Value.IsEmpty())
		{
			MetaArgs.Add(TEXT("m") + Meta.Key.ToString(), FText::FromString(Meta.Value));
		}
	}
	
	return FText::Format(InTextFmt, MetaArgs);
}

FText UInventoryItemData::GetDisplayName(const TMap<FName, FString>& InMetadata) const
{
	FText TextFmt = DisplayName;
	if (const FString* NameKey = InMetadata.Find(InventoryNativeMeta::NameID))
	{
		if (const FInvItemAltName* AltNames = GetFirstAttribute<FInvItemAltName>())
		{
			AltNames->GetName(*NameKey, TextFmt);
		}
	}
	
	return InjectMetadataToText(TextFmt, InMetadata);
}

FText UInventoryItemData::GetDescription(const TMap<FName, FString>& InMetadata) const
{
	FText TextFmt = Description;
	if (const FString* NameKey = InMetadata.Find(InventoryNativeMeta::DescID))
	{
		if (const FInvItemAltDescription* AltDescriptions = GetFirstAttribute<FInvItemAltDescription>())
		{
			AltDescriptions->GetDescription(*NameKey, TextFmt);
		}
	}
	
	return InjectMetadataToText(TextFmt, InMetadata);
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
	else UpdateEditorPreviews();
}
#endif
