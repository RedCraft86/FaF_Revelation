// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryItemData.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_InvMeta, "InventoryMeta");
UE_DEFINE_GAMEPLAY_TAG(Tag_InvNameID, "InventoryMeta.NameID");
UE_DEFINE_GAMEPLAY_TAG(Tag_InvDescID, "InventoryMeta.DescID");
UE_DEFINE_GAMEPLAY_TAG(Tag_InvMeshID, "InventoryMeta.MeshID");
UE_DEFINE_GAMEPLAY_TAG(Tag_InvKeyID, "InventoryMeta.KeyID");

UInventoryItemData::UInventoryItemData() : Priority(1), DisplayName(NSLOCTEXT("Toro", "GenericItemName", "Generic Item"))
	, Description(NSLOCTEXT("Toro", "GenericItemDesc", "This is a generic item!")), StackingMode(EInventoryStackType::UntilMax)
	, StackingValue(5), ItemType(EInventoryItemType::Uncategorized), PreviewZoom({0.5f, 2.0f})
{
}

FText InjectMetadataToText(const FText& InTextFmt, const TMap<FGameplayTag, FString>& InMetadata)
{
	FFormatNamedArguments MetaArgs;
	for (const TPair<FGameplayTag, FString>& Meta : InMetadata)
	{
		if (Meta.Key.IsValid() && Meta.Key != Tag_InvMeta && !Meta.Value.IsEmpty())
		{
			const FString MetaName = Meta.Key.ToString().Replace(TEXT("InventoryMeta."), TEXT(""));
			MetaArgs.Add(TEXT("m") + MetaName, FText::FromString(Meta.Value));
		}
	}
	
	return FText::Format(InTextFmt, MetaArgs);
}

FText UInventoryItemData::GetDisplayName(const TMap<FGameplayTag, FString>& InMetadata) const
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

FText UInventoryItemData::GetDescription(const TMap<FGameplayTag, FString>& InMetadata) const
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
