// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PrimitiveData.h"
#include "Engine/DataAsset.h"
#include "InventoryItemAttributes.h"
#include "StructUtils/InstancedStruct.h"
#include "InventoryItemData.generated.h"

#define ITEM_LIMIT_MAX 9999

namespace InventoryNativeMeta
{
	static inline FName NameID		(TEXT("NameID"));
	static inline FName DescID		(TEXT("DescID"));
	static inline FName MeshID		(TEXT("MeshID"));
	static inline FName KeyID		(TEXT("KeyID"));
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
	Uncategorized,
	Objective,
	KeyItem,
	Consumable,
	Equipment,
	Viewable,
	Custom
};

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
		case EInventoryItemType::Custom:
			return TEXT("Custom");
		default: return TEXT("Uncategorized");
	}
}

UCLASS(NotBlueprintable, BlueprintType)
class TOROGAMEPLAY_API UInventoryItemData : public UDataAsset
{
	GENERATED_BODY()

public:

	UInventoryItemData();

	UPROPERTY(EditAnywhere, Category = General)
		uint8 Priority;

	UPROPERTY(EditAnywhere, Category = General)
		FGuid UniqueID;

	UPROPERTY(EditAnywhere, Category = General)
		TObjectPtr<UTexture2D> Thumbnail;

	// Use "{mKEY}" where KEY is a metadata key to inject custom variables.
	UPROPERTY(EditAnywhere, Category = General)
		FText DisplayName;

	// Use "{mKEY}" where KEY is a metadata key to inject custom variables.
	UPROPERTY(EditAnywhere, Category = General, meta = (MultiLine = true))
		FText Description;
	
	UPROPERTY(EditAnywhere, Category = General, DisplayName = "Stacking")
		EInventoryStackType StackingMode;
	
	UPROPERTY(EditAnywhere, Category = General, meta = (ClampMin = 1, UIMin = 1, EditCondition = "StackingMode != EInventoryStackType::UntilMax"))
		uint8 StackingValue;

	UPROPERTY(EditAnywhere, Category = General)
		EInventoryItemType Category;
	
	UPROPERTY(EditAnywhere, Category = General, meta = (EditCondition = "ItemType == EInventoryItemType::Custom"))
		FText CategoryValue;
	
	UPROPERTY(EditAnywhere, Category = Mesh)
		FVector2D PreviewZoomRange;
	
	UPROPERTY(EditAnywhere, Category = Mesh)
		FTransformMeshData BaseMesh;
	
	UPROPERTY(EditAnywhere, Category = Advanced, meta = (MultiLine = true))
		TMap<FName, FString> DefaultMetadata;

	UPROPERTY(EditAnywhere, Category = Advanced, NoClear, meta = (ExcludeBaseStruct, HideViewOptions))
		TArray<TInstancedStruct<FInventoryItemAttribute>> Attributes;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = Editor)
		FText DisplayNamePreview;
	
	UPROPERTY(VisibleAnywhere, Category = Editor)
		FText DescriptionPreview;
	
	UPROPERTY(VisibleAnywhere, Category = Editor)
		TMap<FName, FString> MetadataKeyGuide;
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

	FText GetDisplayName(const TMap<FName, FString>& InMetadata) const;
	FText GetDescription(const TMap<FName, FString>& InMetadata) const;

#if WITH_EDITOR
	void UpdateEditorPreviews();
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
