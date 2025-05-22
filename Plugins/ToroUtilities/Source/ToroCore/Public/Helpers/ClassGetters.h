// Copyright (Class) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/LocalPlayer.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#define SETTING_GETTER(Class) static const Class* Get() { return GetDefault<Class>(); }
#define SETTING_GETTER_MUTABLE(Class) static Class* Get() { return GetMutableDefault<Class>(); }

#define ENGINE_SUBSYSTEM_GETTER(Class) static Class* Get() { return GEngine ? GEngine->GetEngineSubsystem<Class>() : nullptr; }
#define EDITOR_SUBSYSTEM_GETTER(Class) static Class* Get() { return GEditor ? GEditor->GetEditorSubsystem<Class>() : nullptr; }

#define LOCAL_PLAYER_SUBSYSTEM_GETTER(Class) \
	static Class* Get(const UObject* WorldContext, const int PlayerIndex = 0) \
	{ \
		const APlayerController* Controller = UGameplayStatics::GetPlayerController(WorldContext, PlayerIndex); \
		const ULocalPlayer* LocalPlayer = IsValid(Controller) ? Controller->GetLocalPlayer() : nullptr; \
		return IsValid(LocalPlayer) ? LocalPlayer->GetSubsystem<Class>() : nullptr; \
	}

#define GAME_INSTANCE_SUBSYSTEM_GETTER(Class) \
	static Class* Get(const UObject* WorldContext) \
	{ \
		const UWorld* World = IsValid(WorldContext) ? GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull) : nullptr; \
		const UGameInstance* GI = IsValid(World) ? World->GetGameInstance() : nullptr; \
		return IsValid(GI) ? GI->GetSubsystem<Class>() : nullptr; \
	}

#define WORLD_SUBSYSTEM_GETTER(Class) \
	static Class* Get(const UObject* WorldContext) \
	{ \
		const UWorld* World = IsValid(WorldContext) ? GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull) : nullptr; \
		return IsValid(World) ? World->GetSubsystem<Class>() : nullptr; \
	}

#define GAMEPLAY_ACTOR_GETTER(Class, Func) \
	template <typename T = Class> \
	static T* Get(const UObject* ContextObject) \
	{ \
		return Cast<T>(UGameplayStatics::##Func(ContextObject)); \
	}

#define PLAYER_ACTOR_GETTER(Class, Func) \
	template <typename T = Class> \
	static T* Get(const UObject* ContextObject, const int32 PlayerIndex = 0) \
	{ \
		return Cast<T>(UGameplayStatics::##Func(ContextObject, PlayerIndex)); \
	}