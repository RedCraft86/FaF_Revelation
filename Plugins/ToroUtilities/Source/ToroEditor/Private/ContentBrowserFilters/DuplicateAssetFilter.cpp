// Copyright (C) RedCraft86. All Rights Reserved.

#include "ContentBrowserFilters/DuplicateAssetFilter.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "FrontendFilterBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DuplicateAssetFilter)

#define LOCTEXT_NAMESPACE "ToroEditor"

class FFrontendFilter_DuplicateAsset final : public FFrontendFilter
{
public:
	
	explicit FFrontendFilter_DuplicateAsset(const TSharedPtr<FFrontendFilterCategory>& InCategory)
		: FFrontendFilter(InCategory), bActive(false)
	{
		PopulateDuplicateNames();
		if (const FAssetRegistryModule* Module = FModuleManager::LoadModulePtr<FAssetRegistryModule>("AssetRegistry"))
		{
			IAssetRegistry& AssetRegistry = Module->Get();
			OnAssetAdded = AssetRegistry.OnAssetAdded().AddRaw(this, &FFrontendFilter_DuplicateAsset::OnAssetIO);
			OnAssetRemoved = AssetRegistry.OnAssetRemoved().AddRaw(this, &FFrontendFilter_DuplicateAsset::OnAssetIO);
			OnAssetRenamed = AssetRegistry.OnAssetRenamed().AddRaw(this, &FFrontendFilter_DuplicateAsset::OnAssetRename);
		}
	}

	virtual ~FFrontendFilter_DuplicateAsset() override
	{
		SourcePaths.Empty();
		DuplicateNames.Empty();
		if (const FAssetRegistryModule* Module = FModuleManager::GetModulePtr<FAssetRegistryModule>("AssetRegistry"))
		{
			IAssetRegistry& AssetRegistry = Module->Get();
			AssetRegistry.OnAssetAdded().Remove(OnAssetAdded);
			AssetRegistry.OnAssetRemoved().Remove(OnAssetRemoved);
			AssetRegistry.OnAssetRenamed().Remove(OnAssetRenamed);
		}
	}

	// FFrontendFilter implementation
	virtual FLinearColor GetColor() const override { return FLinearColor::Red; }
	virtual FString GetName() const override { return TEXT("DuplicateAssetFilter"); }
	virtual FText GetDisplayName() const override { return LOCTEXT("DuplicateAssetFilterName", "Duplicate Assets"); }
	virtual FText GetToolTipText() const override { return LOCTEXT("DuplicateAssetFilterTip", "Filters assets that share the same names."); }
	virtual void SetCurrentFilter(TArrayView<const FName> InSourcePaths, const FContentBrowserDataFilter& InBaseFilter) override
	{
		SourcePaths = InSourcePaths;
		PopulateDuplicateNames();
	}
	virtual void ActiveStateChanged(bool InActive) override
	{
		bActive = InActive;
		PopulateDuplicateNames();
	}
	// End of FFrontendFilter implementation

	// IFilter implementation
	virtual bool PassesFilter(FAssetFilterType InItem) const override
	{
		return DuplicateNames.Contains(InItem.GetItemName());
	}
	// End of IFilter implementation

protected:

	bool bActive;
	TArray<FName> SourcePaths;
	TArray<FName> DuplicateNames;
	FDelegateHandle OnAssetAdded, OnAssetRemoved, OnAssetRenamed;
	
	bool IsValidAssetData(const FAssetData& InData)
	{
		if (InData.IsRedirector()) return false;
		
		const FString PathStr = InData.GetObjectPathString();
		if (!PathStr.StartsWith("/Game/")) return false;
		
		for (const FName& SrcPath : SourcePaths)
		{
			if (PathStr.StartsWith(SrcPath.ToString() / TEXT(""))) return true;
		}
		return SourcePaths.IsEmpty();
	}
	
	void PopulateDuplicateNames()
	{
		if (!bActive) return;

		DuplicateNames.Empty();
		if (const FAssetRegistryModule* Module = FModuleManager::LoadModulePtr<FAssetRegistryModule>("AssetRegistry"))
		{
			TArray<FAssetData> AllAssets;
			Module->Get().GetAllAssets(AllAssets);

			TMap<FName, uint8> NameCountMap;
			for (const FAssetData& Data : AllAssets)
			{
				if (!IsValidAssetData(Data)) continue;
				NameCountMap.FindOrAdd(Data.AssetName)++;
			}

			for (const TPair<FName, uint8>& Pair : NameCountMap)
			{
				if (Pair.Value > 1) DuplicateNames.Add(Pair.Key);
			}
		}
	}

	void OnAssetIO(const FAssetData& Asset) { PopulateDuplicateNames(); }
	void OnAssetRename(const FAssetData& Asset, const FString& Name) { PopulateDuplicateNames(); }
};

void UDuplicateAssetFilter::AddFrontEndFilterExtensions(TSharedPtr<FFrontendFilterCategory> DefaultCategory, TArray<TSharedRef<FFrontendFilter>>& InOutFilterList) const
{
	InOutFilterList.Add(MakeShareable(new FFrontendFilter_DuplicateAsset(DefaultCategory)));
}

#undef LOCTEXT_NAMESPACE