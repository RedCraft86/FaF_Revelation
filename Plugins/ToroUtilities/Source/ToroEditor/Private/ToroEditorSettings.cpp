// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEditorSettings.h"

UToroEditorSettings::UToroEditorSettings()
	: StartupCommands({
		{TEXT("r.VSyncEditor"), TEXT("1")},
		{TEXT("r.Streaming.PoolSize"), TEXT("3000")}
	}), AssetLibrary(TEXT("D:/UnrealEngine/Shared/AssetProject/Content/AssetPacks"))
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("ToroEditor");
}

FString UToroEditorSettings::GetAssetLibraryPath() const
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

void UToroEditorSettings::PostInitProperties()
{
	Super::PostInitProperties();
	AssetLibrary.Path = GetAssetLibraryPath();
}

void UToroEditorSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	AssetLibrary.Path = GetAssetLibraryPath();
}
