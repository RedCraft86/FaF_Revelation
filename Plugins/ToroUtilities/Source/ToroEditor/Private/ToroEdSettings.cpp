// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEdSettings.h"

UToroEdSettings::UToroEdSettings()
	: StartupCommands({
		{TEXT("r.VSyncEditor"), TEXT("1")},
		{TEXT("r.Streaming.PoolSize"), TEXT("3000")}
	}), AssetLibrary(TEXT("D:/UnrealEngine/Shared/AssetProject/Content/AssetPacks"))
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("ToroEditor");
}

FString UToroEdSettings::GetAssetLibraryPath() const
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

void UToroEdSettings::PostInitProperties()
{
	Super::PostInitProperties();
	AssetLibrary.Path = GetAssetLibraryPath();
}

void UToroEdSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	AssetLibrary.Path = GetAssetLibraryPath();
}
