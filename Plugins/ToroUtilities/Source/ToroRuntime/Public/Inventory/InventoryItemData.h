// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PrimitiveData.h"
#include "Engine/DataAsset.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "InventoryItemAttributes.h"
#include "StructUtils/InstancedStruct.h"
#include "InventoryItemData.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvMeta);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvNameID);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvDescID);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvMeshID);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvKeyID);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvReuseKey);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvJsonData);

inline bool VerifyInvMetaTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_InvMeta;
}

UENUM(BlueprintType)
enum class EInventoryStackType : uint8
{
	UntilMax,
	DefinedCount,
	UniqueSlots
};

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
	Any,
	Objective,
	KeyItem,
	Consumable,
	Equipment,
	Viewable,
	Uncategorized
};
ENUM_RANGE_BY_FIRST_AND_LAST(EInventoryItemType, EInventoryItemType::Any, EInventoryItemType::Uncategorized);

inline FString LexToString(const EInventoryItemType& InType)
{
	switch (InType)
	{
		case EInventoryItemType::Objective:
			return TEXT("Objective");
		case EInventoryItemType::KeyItem:
			return TEXT("KeyItem");
		case EInventoryItemType::Consumable:
			return TEXT("Consumable");
		case EInventoryItemType::Equipment:
			return TEXT("Equipment");
		case EInventoryItemType::Viewable:
			return TEXT("Viewable");
		case EInventoryItemType::Uncategorized:
			return TEXT("Uncategorized");
		default: return TEXT("All");
	}
}

USTRUCT(BlueprintType)
struct TORORUNTIME_API FInventoryMetadata
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Metadata, meta = (MultiLine = true, ForceInlineRow, Categories = "InventoryMeta"))
		TMap<FGameplayTag, FString> Metadata;

	FInventoryMetadata() : Metadata({}) {}
	FInventoryMetadata(const TMap<FGameplayTag, FString>& InMetadata) : Metadata(InMetadata) {}
	friend FArchive& operator<<(FArchive& Ar, FInventoryMetadata& InMetadata)
	{
		Ar << InMetadata.Metadata;
		return Ar;
	}

	TMap<FGameplayTag, FString>& operator*() { return Metadata; }
	const TMap<FGameplayTag, FString>& operator*() const { return Metadata; }

	void Validate();
	void Remove(const FGameplayTag& InKey);
	void Add(const FGameplayTag& InKey, const FString& InValue);
	void Append(const FInventoryMetadata& InMetadata);

	bool Contains(const FGameplayTag& InKey) const { return Metadata.Contains(InKey); }
	bool HasMetadata(const TPair<FGameplayTag, FString>& InMetadata, const bool bAnyValue = true) const;
	bool HasAnyMetadata(const TMap<FGameplayTag, FString>& InMetadata, const bool bAnyValue = true) const;
	bool HasAllMetadata(const TMap<FGameplayTag, FString>& InMetadata, const bool bAnyValue = true) const;

	void ForEach(const TFunctionRef<void(const FGameplayTag& Key, const FString& Value)>& Func) const;
	const FString* Find(const FGameplayTag& InKey) const { return Metadata.Find(InKey); }
	FString FindRef(const FGameplayTag& InKey) const { return Metadata.FindRef(InKey); }
	
	static FString GetKeyName(const FGameplayTag& InKey)
	{
		return InKey.ToString().Replace(TEXT("InventoryMeta."), TEXT(""));
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UInventoryItemData final : public UDataAsset
{
	GENERATED_BODY()

public:

	UInventoryItemData();

	UPROPERTY(EditAnywhere, Category = General)
		uint8 Priority;

	UPROPERTY(EditAnywhere, Category = General)
		TSoftObjectPtr<UTexture2D> Thumbnail;

	// Use "{mKEY}" where KEY is a metadata key to inject custom variables.
	UPROPERTY(EditAnywhere, Category = General)
		FText DisplayName;

	// Use "{mKEY}" where KEY is a metadata key to inject custom variables.
	UPROPERTY(EditAnywhere, Category = General, meta = (MultiLine = true))
		FText Description;
	
	UPROPERTY(EditAnywhere, Category = General)
		EInventoryStackType StackingMode;
	
	UPROPERTY(EditAnywhere, Category = General, meta = (ClampMin = 1, UIMin = 1, EditCondition = "StackingMode != EInventoryStackType::UntilMax"))
		uint8 StackingValue;

	UPROPERTY(EditAnywhere, Category = General)
		EInventoryItemType ItemType;
	
	UPROPERTY(EditAnywhere, Category = General, meta = (EditCondition = "ItemType == EInventoryItemType::Uncategorized"))
		FString CustomType;
	
	UPROPERTY(EditAnywhere, Category = Mesh, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		FVector2D PreviewZoom;
	
	UPROPERTY(EditAnywhere, Category = Mesh)
		FTransformMeshData BaseMesh;
	
	UPROPERTY(EditAnywhere, Category = Advanced)
		FInventoryMetadata DefaultMetadata;

	UPROPERTY(EditAnywhere, Category = Advanced, NoClear, meta = (ExcludeBaseStruct, HideViewOptions))
		TArray<TInstancedStruct<FInventoryItemAttribute>> Attributes;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = Editor, Transient)
		FText DisplayNamePreview;
	
	UPROPERTY(VisibleAnywhere, Category = Editor, Transient)
		FText DescriptionPreview;
	
	UPROPERTY(VisibleAnywhere, Category = Editor, Transient, meta = (MultiLine = true, ForceInlineRow))
	TMap<FGameplayTag, FString> MetadataKeyGuide{
		{Tag_InvNameID,	TEXT("Used with the Alt Names Attribute.")},
		{Tag_InvDescID,	TEXT("Used with the Alt Descriptions Attribute.")},
		{Tag_InvMeshID,	TEXT("Used with the Alt Meshes Attribute.")},
		{Tag_InvKeyID,		TEXT("This item will be able to unlock locks with matching IDs.")},
		{Tag_InvReuseKey,	TEXT("Used for key types. Signifies that a key is reusable for other locks.")},
		{Tag_InvJsonData,	TEXT("Used for equipment types to store extra equipment data.")}
	};
#endif

	template<typename T = FInventoryItemAttribute>
	const T* GetFirstAttribute() const
	{
		for (const TInstancedStruct<FInventoryItemAttribute>& Attribute : Attributes)
		{
			if (Attribute.GetScriptStruct() == T::StaticStruct())
			{
				return Attribute.GetPtr<T>();
			}
		}
		
		return nullptr;
	}

	uint8 GetSlotLimit() const;
	uint8 GetStackLimit() const;
	FString GetCategoryName() const;
	FText GetDisplayName(const FInventoryMetadata& InMetadata) const;
	FText GetDescription(const FInventoryMetadata& InMetadata) const;
	FTransformMeshData GetMeshData(const FInventoryMetadata& InMetadata) const;
	static FText InjectMetadataToText(const FText& InTextFmt, const FInventoryMetadata& InMetadata);

#if WITH_EDITOR
	void UpdateEditorPreviews();
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
