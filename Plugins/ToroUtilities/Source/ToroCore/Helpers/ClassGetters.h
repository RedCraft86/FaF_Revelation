// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/LocalPlayer.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#define SETTING_GETTER(C) static const C* Get() { return GetDefault<C>(); }
#define SETTING_GETTER_MUTABLE(C) static C* Get() { return GetMutableDefault<C>(); }

#define ENGINE_SUBSYSTEM_GETTER(C) static C* Get() { return GEngine ? GEngine->GetEngineSubsystem<C>() : nullptr; }
#define EDITOR_SUBSYSTEM_GETTER(C) static C* Get() { return GEditor ? GEditor->GetEditorSubsystem<C>() : nullptr; }

#define LOCAL_PLAYER_SUBSYSTEM_GETTER(C) \
	static C* Get(const UObject* WorldContext, const int PlayerIndex = 0) \
	{ \
		const APlayerController* Controller = UGameplayStatics::GetPlayerController(WorldContext, PlayerIndex); \
		const ULocalPlayer* LocalPlayer = IsValid(Controller) ? Controller->GetLocalPlayer() : nullptr; \
		return IsValid(LocalPlayer) ? LocalPlayer->GetSubsystem<C>() : nullptr; \
	}

#define GAME_INSTANCE_SUBSYSTEM_GETTER(C) \
	static C* Get(const UObject* WorldContext) \
	{ \
		const UWorld* World = IsValid(WorldContext) ? GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull) : nullptr; \
		const UGameInstance* GI = IsValid(World) ? World->GetGameInstance() : nullptr; \
		return IsValid(GI) ? GI->GetSubsystem<C>() : nullptr; \
	}

#define WORLD_SUBSYSTEM_GETTER(C) \
	static C* Get(const UObject* WorldContext) \
	{ \
		const UWorld* World = IsValid(WorldContext) ? GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull) : nullptr; \
		return IsValid(World) ? World->GetSubsystem<C>() : nullptr; \
	}
