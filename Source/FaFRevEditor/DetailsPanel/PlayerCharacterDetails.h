// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "DetailCategoryBuilder.h"
#include "Player/PlayerCharacter.h"
#include "DetailsPanels/CustomizationHelpers.h"
#include "DetailsPanels/ToroCharacterDetails.h"

#define CLASSNAME APlayerCharacter
class FPlayerCharacterDetails final : public FToroCharacterDetails
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FPlayerCharacterDetails>();
	}

protected:

	virtual void PostApplyCustomization(IDetailLayoutBuilder& DetailBuilder) override
	{
		IDetailCategoryBuilder& SettingsCat = DetailBuilder.EditCategory("Settings");
		{
			SettingsCat.AddProperty(CLASS_PROPERTY_NS(CharacterID));
			SettingsCat.AddProperty(CLASS_PROPERTY_NS(LockTags));
			SettingsCat.AddProperty(CLASS_PROPERTY_NS(ControlFlags));
			SettingsCat.AddProperty(CLASS_PROPERTY_NS(StateFlags));
			SettingsCat.AddProperty(CLASS_PROPERTY_NS(LockOnSpeed));
		}

		IDetailGroup& InteractGroup = SettingsCat.AddGroup("Interaction", INVTEXT("Interaction"));
		{
			InteractGroup.HeaderProperty(CLASS_PROPERTY_NS(InteractDistance)).DisplayName(INVTEXT("Interaction"));
			InteractGroup.AddPropertyRow(CLASS_PROPERTY_NS(InteractTrace)).DisplayName(INVTEXT("Trace Channel"));
		}

		IDetailGroup& LeanGroup = SettingsCat.AddGroup("Leaning", INVTEXT("Leaning"));
		{
			CLASS_PROPERTY_VAR_NS(LeanOffsets, LeanOffsets)
		   LeanGroup.HeaderRow().NameContent()[ SIMPLE_LABEL(INVTEXT("Leaning")) ]
		   .ValueContent()
		   [
			   CustomizationHelpers::ConstructHorizontalVec2D(LeanOffsets, INVTEXT("Pos"), INVTEXT("Rot"))
		   ];
			LeanGroup.AddPropertyRow(CLASS_PROPERTY_NS(SideTrace)).DisplayName(INVTEXT("Trace Channel"));
		}

		IDetailGroup& MoveGroup = SettingsCat.AddGroup("Movement", INVTEXT("Movement"));
		{
			MoveGroup.HeaderProperty(CLASS_PROPERTY_NS(WalkSpeed)).DisplayName(INVTEXT("Walk Speed"));
			CLASS_PROPERTY_VAR_NS(StrafeOffsets, StrafeOffsets)
			MoveGroup.AddWidgetRow().NameContent()[ SIMPLE_LABEL(INVTEXT("Camera Tilt")) ]
			.ValueContent()
			[
				CustomizationHelpers::ConstructHorizontalVec2D(StrafeOffsets, INVTEXT("Pos"), INVTEXT("Rot"))
			];
		}

		IDetailGroup& RunGroup = SettingsCat.AddGroup("Running", INVTEXT("Running"));
		{
			CLASS_PROPERTY_VAR_NS(RunConfig, RunConfig);
			RunGroup.HeaderProperty(RunConfig->GetChildHandle(0).ToSharedRef()).DisplayName(INVTEXT("Run Multiplier"));
			RunGroup.AddPropertyRow(RunConfig->GetChildHandle(1).ToSharedRef()).DisplayName(INVTEXT("FOV Offset"));
		}
		
		IDetailGroup& StaminaGroup = RunGroup.AddGroup("Stamina", INVTEXT("Stamina"));
		{
			CLASS_PROPERTY_VAR_NS(Stamina, Stamina)
			StaminaGroup.AddPropertyRow(Stamina->GetChildHandle(0).ToSharedRef()).DisplayName(INVTEXT("Interval"));

			IDetailGroup& StaminaNormalGroup = StaminaGroup.AddGroup("StaminaNormal", INVTEXT("Normal"));
			StaminaNormalGroup.HeaderProperty(Stamina->GetChildHandle(1).ToSharedRef()).DisplayName(INVTEXT("Normal Max"));
			StaminaNormalGroup.AddWidgetRow().NameContent()[ SIMPLE_LABEL(INVTEXT("Rates")) ]
			.ValueContent()
			[
				CustomizationHelpers::ConstructHorizontalVec2D(
					Stamina->GetChildHandle(2).ToSharedRef(),
					INVTEXT("Drain"), INVTEXT("Gain"))
			];

			IDetailGroup& StaminaReserveGroup = StaminaGroup.AddGroup("StaminaReserve", INVTEXT("Reserve"));
			StaminaReserveGroup.HeaderProperty(Stamina->GetChildHandle(3).ToSharedRef()).DisplayName(INVTEXT("Reserve Max"));
			StaminaReserveGroup.AddWidgetRow().NameContent()[ SIMPLE_LABEL(INVTEXT("Rates")) ]
			.ValueContent()
			[
				CustomizationHelpers::ConstructHorizontalVec2D(
					Stamina->GetChildHandle(4).ToSharedRef(),
					INVTEXT("Drain"), INVTEXT("Gain"))
			];
		}

		IDetailGroup& SneakGroup = SettingsCat.AddGroup("Sneaking", INVTEXT("Sneaking"));
		{
			CLASS_PROPERTY_VAR_NS(SneakConfig, SneakConfig);
			SneakGroup.HeaderProperty(SneakConfig->GetChildHandle(0).ToSharedRef()).DisplayName(INVTEXT("Sneak Multiplier"));
			SneakGroup.AddPropertyRow(SneakConfig->GetChildHandle(1).ToSharedRef()).DisplayName(INVTEXT("FOV Offset"));
			CLASS_PROPERTY_VAR_NS(HalfHeights, HalfHeights);
			SneakGroup.AddWidgetRow().NameContent()[ SIMPLE_LABEL(INVTEXT("Height")) ]
			.ValueContent()
			[
				CustomizationHelpers::ConstructVerticalVec2D(HalfHeights, INVTEXT("Stand"), INVTEXT("Crouch"))
			];
			SneakGroup.AddPropertyRow(CLASS_PROPERTY_NS(CeilingTrace)).DisplayName(INVTEXT("Trace Channel"));
		}
	}
};
#undef CLASSNAME