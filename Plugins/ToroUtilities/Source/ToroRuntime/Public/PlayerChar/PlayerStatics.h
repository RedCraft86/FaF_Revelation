﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
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
		inline static FName RunFOV		= TEXT("Internal_RunFOV");
		inline static FName CrouchFOV	= TEXT("Internal_CrouchFOV");
	}

	namespace LockFlags
	{
		static TSet<FName> GetAll()
		{
			return {
				Tag_LockMainMenu.GetTag().GetTagName(),
				Tag_LockStartup.GetTag().GetTagName(),
				Tag_LockLoading.GetTag().GetTagName(),
				Tag_LockCinematic.GetTag().GetTagName(),
				Tag_LockDialogue.GetTag().GetTagName(),
				Tag_LockJumpscare.GetTag().GetTagName(),
				Tag_LockInventory.GetTag().GetTagName(),
				Tag_LockHiding.GetTag().GetTagName(),
				Tag_LockDevice.GetTag().GetTagName(),
				Tag_LockGuide.GetTag().GetTagName(),
				Tag_LockQTE.GetTag().GetTagName()
			};
		}

		static TSet<FName> Immunity()
		{
			return {
				Tag_LockMainMenu.GetTag().GetTagName(),
				Tag_LockStartup.GetTag().GetTagName(),
				Tag_LockLoading.GetTag().GetTagName(),
				Tag_LockCinematic.GetTag().GetTagName(),
				Tag_LockDialogue.GetTag().GetTagName(),
				Tag_LockJumpscare.GetTag().GetTagName(),
				Tag_LockHiding.GetTag().GetTagName(),
				Tag_LockGuide.GetTag().GetTagName()
			};
		}

		static TSet<FName> Resettable()
		{
			return {
				Tag_LockInventory.GetTag().GetTagName(),
				Tag_LockHiding.GetTag().GetTagName(),
				Tag_LockDevice.GetTag().GetTagName(),
				Tag_LockQTE.GetTag().GetTagName()
			};
		}
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

#define LockFlag(Flag) Tag_Lock##Flag.GetTag().GetTagName()

#define BIND_INPUT_ACTION(Component, Event, Action) \
	if (const UInputAction* IA_##Action = InputActions.FindRef(Player::InputActions::Action)) \
	{ Component->BindAction(IA_##Action, ETriggerEvent::Event, this, &ThisClass::InputBinding_##Action); }

#define INPUT_BINDING_FUNCTIONS() \
	void InputBinding_Pause(const FInputActionValue& InValue); \
	void InputBinding_Turn(const FInputActionValue& InValue); \
	void InputBinding_Move(const FInputActionValue& InValue); \
	void InputBinding_Run(const FInputActionValue& InValue); \
	void InputBinding_Crouch(const FInputActionValue& InValue); \
	void InputBinding_Lean(const FInputActionValue& InValue); \
	void InputBinding_Inventory(const FInputActionValue& InValue); \
	void InputBinding_HideQuests(const FInputActionValue& InValue); \
	void InputBinding_Interact(const FInputActionValue& InValue); \
	void InputBinding_Equipment(const FInputActionValue& InValue); \
	void InputBinding_EquipmentAlt(const FInputActionValue& InValue);

#define INPUT_EVENT_BINDS(Component) \
	BIND_INPUT_ACTION(Component, Started, Pause) \
	BIND_INPUT_ACTION(Component, Triggered, Turn) \
	BIND_INPUT_ACTION(Component, Triggered, Move) \
	BIND_INPUT_ACTION(Component, Completed, Move) \
	BIND_INPUT_ACTION(Component, Canceled, Move) \
	BIND_INPUT_ACTION(Component, Started, Run) \
	BIND_INPUT_ACTION(Component, Completed, Run) \
	BIND_INPUT_ACTION(Component, Canceled, Run) \
	BIND_INPUT_ACTION(Component, Started, Crouch) \
	BIND_INPUT_ACTION(Component, Started, Lean) \
	BIND_INPUT_ACTION(Component, Completed, Lean) \
	BIND_INPUT_ACTION(Component, Canceled, Lean) \
	BIND_INPUT_ACTION(Component, Started, Inventory) \
	BIND_INPUT_ACTION(Component, Started, HideQuests) \
	BIND_INPUT_ACTION(Component, Triggered, Interact) \
	BIND_INPUT_ACTION(Component, Completed, Interact) \
	BIND_INPUT_ACTION(Component, Canceled, Interact) \
	BIND_INPUT_ACTION(Component, Started, Equipment) \
	BIND_INPUT_ACTION(Component, Started, EquipmentAlt) \
	BIND_INPUT_ACTION(Component, Completed, EquipmentAlt) \
	BIND_INPUT_ACTION(Component, Canceled, EquipmentAlt)