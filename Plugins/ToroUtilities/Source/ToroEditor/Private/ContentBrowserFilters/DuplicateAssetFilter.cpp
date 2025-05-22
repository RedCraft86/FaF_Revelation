// Copyright (C) RedCraft86. All Rights Reserved.

#include "ContentBrowserFilters/DuplicateAssetFilter.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "FrontendFilterBase.h"
#include "ToroEdSettings.h"
#include "ToroEditor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DuplicateAssetFilter)

#define LOCTEXT_NAMESPACE "ToroEditor"

class FFrontendFilter_DuplicateAsset final : public FFrontendFilter
{
public:
	
	inline static int64 SizeTolerance = 0;
	static int64 GetAssetSize(const FAssetData& InAsset)
	{
		if (!InAsset.IsValid()) return 0;
		return IFileManager::Get().FileSize(*FPackageName::LongPackageNameToFilename(
			InAsset.PackageName.ToString(), TEXT(".uasset")));
	}
	static bool CompareSizes(const int64& A, const int64& B)
	{
		return SizeTolerance < 0 ? true : FMath::Abs(A - B) <= SizeTolerance;
	}
	
	struct FDuplicateAssetData
	{
		TSet<FString> Dupes;
		TMap<FAssetData, int64> Assets;
		void AddAsset(const FAssetData& InAsset)
		{
			const int64 Size = GetAssetSize(InAsset);
			for (const TPair<FAssetData, int64>& Asset : Assets)
			{
				if (Asset.Key.AssetClassPath == InAsset.AssetClassPath && CompareSizes(Asset.Value, Size))
				{
					Dupes.Add(*InAsset.GetSoftObjectPath().ToString());
					Dupes.Add(*Asset.Key.GetSoftObjectPath().ToString());
				}
			}
			
			Assets.Add(InAsset, Size);
		}
		void AppendDuplicates(TSet<FString>& InArray) const
		{
			InArray.Append(Dupes);
		}
	};

	bool bActive;
	TSet<FString> Duplicates;
	TArray<FName> SourcePaths;
	FDelegateHandle OnAssetAdded, OnAssetRemoved, OnAssetRenamed;
	
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
		Duplicates.Empty();
		SourcePaths.Empty();
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
	virtual void LoadSettings(const FString& IniFilename, const FString& IniSection, const FString& SettingsString) override
	{
		if (IsActive()) SetActive(false); // Disable if left on to prevent engine startup stalls
	}
	// End of FFrontendFilter implementation

	// IFilter implementation
	virtual bool PassesFilter(FAssetFilterType InItem) const override
	{
		return Duplicates.Contains(InItem.GetInternalPath().ToString());
	}
	// End of IFilter implementation
	
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

		Duplicates.Empty();
		SizeTolerance = UToroEdSettings::Get()->CalcDupliFilterSize();
		if (const FAssetRegistryModule* Module = FModuleManager::LoadModulePtr<FAssetRegistryModule>("AssetRegistry"))
		{
			TArray<FAssetData> AllAssets;
			Module->Get().GetAllAssets(AllAssets);
			if (const int32 NumAssets = AllAssets.Num(); NumAssets > 20000)
			{
				UE_LOG(LogToroEditor, Error, TEXT("Content browser failed to filter duplicates. Too many assets [%d/10,000]"), NumAssets)
				SetActive(false);
			}

			TMap<FName, FDuplicateAssetData> Dupes;
			for (const FAssetData& Data : AllAssets)
			{
				if (!IsValidAssetData(Data)) continue;
				Dupes.FindOrAdd(Data.AssetName).AddAsset(Data);
			}

			for (const TPair<FName, FDuplicateAssetData>& Pair : Dupes)
			{
				Pair.Value.AppendDuplicates(Duplicates);
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