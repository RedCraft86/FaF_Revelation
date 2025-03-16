// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ClassGetterHelpers.h"
#include "Engine/DeveloperSettings.h"
#include "ToroEditorSettings.generated.h"

UENUM(BlueprintInternalUseOnly)
enum class EDupliFilterSizeType : uint8
{
	None,
	Bytes,
	Kilobytes,
	Megabytes
};

UCLASS(Config = Editor, DefaultConfig, DisplayName = "Toro Editor")
class TOROEDITOR_API UToroEditorSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroEditorSettings()
		: StartupCommands({
			  {TEXT("r.VSyncEditor"), TEXT("1")},
			  {TEXT("r.Streaming.PoolSize"), TEXT("3000")}
		}), AssetLibrary(TEXT("D:/UnrealEngine/Shared/AssetProject/Content/AssetPacks"))
		, DFSize(5), DFSizeType(EDupliFilterSizeType::Megabytes)
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroEditor");
	}

	SETTING_GETTER(UToroEditorSettings)

	UPROPERTY(Config, EditAnywhere, Category = Editor)
		TMap<FString, FString> StartupCommands;

private:
	
	UPROPERTY(Config, EditAnywhere, Category = Editor)
		FDirectoryPath AssetLibrary;
	
	UPROPERTY(Config, EditAnywhere, Category = DupliFilter, DisplayName = "Size Threshold", meta = (ClampMin = 0, UIMin = 0))
		int64 DFSize;

	UPROPERTY(Config, EditAnywhere, Category = DupliFilter, DisplayName = "Size Type")
		EDupliFilterSizeType DFSizeType;

public:
	
	int64 CalcDupliFilterSize() const
	{
		switch (DFSizeType)
		{
		case EDupliFilterSizeType::Bytes: return DFSize;
		case EDupliFilterSizeType::Kilobytes: return DFSize * 1024;
		case EDupliFilterSizeType::Megabytes: return DFSize * 1024 * 1024;
		default: return -1;
		}
	}

	FString GetAssetLibraryPath() const
	{
		FString Path = AssetLibrary.Path;
		FPaths::NormalizeDirectoryName(Path);
		if (FPaths::DirectoryExists(Path))
		{
			FString ProjectDir, Remainder;
			if (Path.Split(TEXT("/Content"), &ProjectDir, &Remainder))
			{
				if (!ProjectDir.IsEmpty() && FPaths::DirectoryExists(ProjectDir))
				{
					TArray<FString> Files;
					IFileManager::Get().FindFiles(Files, *ProjectDir, TEXT(".uproject"));
					if (Files.Num() == 1)
					{
						return Path;
					}
				}
			}
		}
		
		return TEXT("");
	}

private:

	virtual void PostInitProperties() override
	{
		Super::PostInitProperties();
		AssetLibrary.Path = GetAssetLibraryPath();
	}

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		AssetLibrary.Path = GetAssetLibraryPath();
	}
};