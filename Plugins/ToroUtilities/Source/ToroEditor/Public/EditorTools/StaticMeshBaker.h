﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditorCommands.h"
#include "Framework/Commands/Commands.h"
#include "PrimitiveData.h"

class FStaticMeshBaker
{
public:

	static void Register(const TSharedPtr<FUICommandList>& Commands)
	{
		MAP_TOOL(StaticMeshBaker, FStaticMeshBaker);
	}
	
	static void RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
	{
		REGISTER_TOP_MENU(StaticMeshBaker)
		REGISTER_TOOLBAR(StaticMeshBaker, Actor)
	}

private:

	static void ExecuteAction();
	static AStaticMeshActor* SpawnActor(class UEditorActorSubsystem* Subsystem, const FStaticMeshProperties& MeshProperties, const FTransform& Transform, const FString& Label, const FString& Folder);
};