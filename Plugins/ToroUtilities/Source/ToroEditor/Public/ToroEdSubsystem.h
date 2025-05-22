// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEdSettings.h"
#include "EditorSubsystem.h"
#include "Helpers/ClassGetters.h"
#include "ToroEdSubsystem.generated.h"

UCLASS()
class TOROEDITOR_API UToroEdSubsystem final : public UEditorSubsystem
{
	GENERATED_BODY()

public:

	UToroEdSubsystem() {}
	EDITOR_SUBSYSTEM_GETTER(UToroEdSubsystem)

	virtual void Initialize(FSubsystemCollectionBase& Collection) override
	{
		Super::Initialize(Collection);
		const UToroEdSettings* Settings = UToroEdSettings::Get();
		for (const TPair<FString, FString>& Command : Settings->StartupCommands)
		{
			UKismetSystemLibrary::ExecuteConsoleCommand(this,
				*FString::Printf(TEXT("%s %s"), *Command.Key, *Command.Value));
		}
	}
};