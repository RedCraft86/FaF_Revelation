// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEdSubsystem.h"
#include "ToroEdSettings.h"

void UToroEdSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const UToroEdSettings* Settings = UToroEdSettings::Get();
	for (const TPair<FString, FString>& Command : Settings->StartupCommands)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(this,
			*FString::Printf(TEXT("%s %s"), *Command.Key, *Command.Value));
	}
}
