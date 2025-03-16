// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EditorSubsystem.h"
#include "ClassGetterHelpers.h"
#include "ToroEditorSubsystem.generated.h"

UCLASS()
class TOROEDITOR_API UToroEditorSubsystem final : public UEditorSubsystem
{
	GENERATED_BODY()

public:

	UToroEditorSubsystem() {}
	EDITOR_SUBSYSTEM_GETTER(UToroEditorSubsystem)

	void RunCommand(const FString& Command) const;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
