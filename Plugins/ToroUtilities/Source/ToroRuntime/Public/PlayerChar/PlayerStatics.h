// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/GenericPlayerTypes.h"

namespace Player
{
	namespace InputActions
	{
		inline static FName Pause			= TEXT("Pause");
		inline static FName Turn			= TEXT("Turn");
		inline static FName Move			= TEXT("Move");
		inline static FName Run				= TEXT("Run");
		inline static FName Crouch			= TEXT("Crouch");
		inline static FName Lean			= TEXT("Lean");
		inline static FName Inventory		= TEXT("Inventory");
		inline static FName HideQuests		= TEXT("HideQuests");
		inline static FName Interact		= TEXT("Interact");
		inline static FName Equipment		= TEXT("Equipment");
		inline static FName EquipmentAlt	= TEXT("EquipmentAlt");

		inline static TSet All = { Pause, Turn, Move, Run, Crouch, Lean,
			Inventory, HideQuests, Interact, Equipment, EquipmentAlt };
	}

	namespace InternalKeys
	{
		inline static FString RunFOV		= TEXT("Internal_RunFOV");
		inline static FString CrouchFOV		= TEXT("Internal_CrouchFOV");
	}

	namespace LockFlags
	{
		inline static TSet<FGameplayTag> All = {
			Tag_LockStartup.GetTag(),
			Tag_LockLoading.GetTag(),
			Tag_LockCutscene.GetTag(),
			Tag_LockDialogue.GetTag(),
			Tag_LockJumpscare.GetTag(),
			Tag_LockInventory.GetTag(),
			Tag_LockHiding.GetTag(),
			Tag_LockDevice.GetTag(),
			Tag_LockGuide.GetTag(),
			Tag_LockQTE.GetTag()
		};

		inline static TSet<FGameplayTag> Immunity = {
			Tag_LockStartup.GetTag(),
			Tag_LockLoading.GetTag(),
			Tag_LockCutscene.GetTag(),
			Tag_LockDialogue.GetTag(),
			Tag_LockJumpscare.GetTag(),
			Tag_LockHiding.GetTag(),
			Tag_LockGuide.GetTag()
		};

		inline static TSet<FGameplayTag> Resettable = {
			Tag_LockInventory.GetTag(),
			Tag_LockHiding.GetTag(),
			Tag_LockDevice.GetTag(),
			Tag_LockQTE.GetTag()
		};
	}

	static float LeanToFloat(const EPlayerLeanState& State)
	{
		switch (State)
		{
		case EPlayerLeanState::Left: return -1.0f;
		case EPlayerLeanState::Right: return 1.0f;
		default: return 0.0f;
		}
	}
}
