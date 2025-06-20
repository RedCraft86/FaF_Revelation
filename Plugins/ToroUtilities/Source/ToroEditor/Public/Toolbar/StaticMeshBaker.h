﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCmds.h"
#include "Framework/Commands/Commands.h"
#include "DataTypes/PrimitiveData.h"

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
	static AStaticMeshActor* SpawnActor(class UEditorActorSubsystem* Subsystem, const FStaticMeshData& MeshData, const FTransform& Transform, const FString& Label, const FString& Folder);
};