// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEditorSubsystem.h"
#include "ToroCoreSettings.h"

void UToroEditorSubsystem::RunCommand(const FString& Command) const
{
	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
	if (!IConsoleManager::Get().ProcessUserConsoleInput(*Command, *GLog, World))
	{
		GEngine->Exec(World, *Command);
	}
}

void UToroEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const TMap<FString, FString> Commands = UToroCoreSettings::Get()->StartupCommands;
	for (const TPair<FString, FString>& Command : Commands)
	{
		RunCommand(FString::Printf(TEXT("%s %s"), *Command.Key, *Command.Value));
	}
}

void UToroEditorSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
