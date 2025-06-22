// Copyright (Class) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/LocalPlayer.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#define SETTING_GETTER(Class) static Class* Get() { return GetMutableDefault<Class>(); }

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

#define GLOBAL_CLASS_GETTER(Class, Func) \
	template <typename T = Class> \
	static T* Get(const UObject* ContextObject) \
	{ \
		return Cast<T>(UGameplayStatics::Func(ContextObject)); \
	}

#define PLAYER_CLASS_GETTER(Class, Func) \
	template <typename T = Class> \
	static T* Get(const UObject* ContextObject, const int32 PlayerIndex = 0) \
	{ \
		return Cast<T>(UGameplayStatics::Func(ContextObject, PlayerIndex)); \
}

#define FIND_COMPONENT(CompClass, OwnerClass, ...) \
	if (const AActor* Actor = OwnerClass::Get(__VA_ARGS__)) \
	{ \
		if (CompClass* Comp = Actor->FindComponentByClass<CompClass>()) \
			return Comp; \
	}

#define FIND_GLOBAL_COMPONENT(Class) \
	FIND_COMPONENT(Class, AToroGameMode, ContextObject) \
	FIND_COMPONENT(Class, AToroGameState, ContextObject) \
	return nullptr;

#define FIND_PLAYER_COMPONENT(Class) \
	FIND_COMPONENT(Class, AToroPlayerCharacter, ContextObject, PlayerIndex) \
	FIND_COMPONENT(Class, AToroPlayerController, ContextObject, PlayerIndex) \
	FIND_COMPONENT(Class, AToroPlayerState, ContextObject, PlayerIndex) \
	return nullptr;

#define FIND_GENERAL_COMPONENT(Class) \
	FIND_COMPONENT(Class, AToroGameMode, ContextObject) \
	FIND_COMPONENT(Class, AToroGameState, ContextObject) \
	FIND_COMPONENT(Class, AToroPlayerCharacter, ContextObject, PlayerIndex) \
	FIND_COMPONENT(Class, AToroPlayerController, ContextObject, PlayerIndex) \
	FIND_COMPONENT(Class, AToroPlayerState, ContextObject, PlayerIndex) \
	return nullptr;