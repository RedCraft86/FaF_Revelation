// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/LocalPlayer.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#define LOCAL_PLAYER_SUBSYSTEM_GETTER(Class) \
	static Class* Get(const UObject* WorldContext, const int PlayerIndex = 0) \
	{ \
		const APlayerController* Controller = UGameplayStatics::GetPlayerController(WorldContext, PlayerIndex); \
		const ULocalPlayer* LocalPlayer = Controller ? Controller->GetLocalPlayer() : nullptr; \
		return LocalPlayer ? LocalPlayer->GetSubsystem<Class>() : nullptr; \
	}

#define GAME_INSTANCE_SUBSYSTEM_GETTER(Class) \
	static Class* Get(const UObject* WorldContext) \
	{ \
		const UWorld* World = WorldContext ? GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull) : nullptr; \
		const UGameInstance* GI = World ? World->GetGameInstance() : nullptr; \
		return GI ? GI->GetSubsystem<Class>() : nullptr; \
	}

#define ENGINE_SUBSYSTEM_GETTER(Class) \
	static Class* Get(const UObject* WorldContext) \
	{ \
		return GEngine ? GEngine->GetEngineSubsystem<Class>() : nullptr; \
	}