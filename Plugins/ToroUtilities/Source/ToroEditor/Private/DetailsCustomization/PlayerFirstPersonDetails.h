// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "DetailsCustomization/ToroActorDetails.h"
#include "PlayerChar/ToroFirstPersonPlayer.h"

#define CLASSNAME AToroFirstPersonPlayer
class TOROEDITOR_API FFirstPersonPlayerCustomization final : public FToroActorCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FFirstPersonPlayerCustomization>();
	}

private:

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		FToroActorCustomization::CustomizeDetails(DetailBuilder);
		DetailBuilder.HideCategory(TEXT("Rendering"));

		IDetailCategoryBuilder& Settings = DetailBuilder.EditCategory(TEXT("Settings"));

		FDetailWidgetRow& InteractRow = Settings.AddCustomRow(INVTEXT("Interact"));
		{
			GET_PROPERTY_VAR(ReachDistance, ReachDistance)
			GET_PROPERTY_VAR(InteractTrace, InteractionTrace)
			InteractRow.OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(ReachDistance))
			.NameContent()
			[
				GENERIC_PROPERTY_LABEL(Interaction)
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 10.0f, 0.0f)
				.MinWidth(125.0f)
				.AutoWidth()
				[
					InteractionTrace->CreatePropertyValueWidget()
				]
				+SHorizontalBox::Slot()
				.MinWidth(125.0f)
				.AutoWidth()
				[
					ReachDistance->CreatePropertyValueWidget()
				]
			];
		}

		IDetailGroup& FOVGroup = Settings.AddGroup(TEXT("FieldOfView"), INVTEXT("Field Of View"));
		{
			GET_PROPERTY_VAR(FieldOfView, FieldOfView)

			const TSharedRef<IPropertyHandle> Base = FieldOfView->GetChildHandle(0).ToSharedRef();
			ADD_CLAMP_META(Base, 60.0, 140.0)
			FOVGroup.HeaderProperty(Base)
				.DisplayName(INVTEXT("Field Of View"));

			FOVGroup.AddPropertyRow(GET_PROPERTY(FieldOfViewSpeed)).DisplayName(INVTEXT("Speed"));
			FOVGroup.AddPropertyRow(FieldOfView->GetChildHandle(1).ToSharedRef()).DisplayName(INVTEXT("Offsets"));
		}

		IDetailGroup& LockOnGroup = Settings.AddGroup(TEXT("LockOn"), INVTEXT("Lock On"));
		{
			LockOnGroup.HeaderProperty(GET_PROPERTY(LockOnSpeed)).DisplayName(INVTEXT("Lock-On Speed"));
			LockOnGroup.AddPropertyRow(GET_PROPERTY(LockOnTarget)).DisplayName(INVTEXT("Target"));
		}

		IDetailGroup& SensitivityGroup = Settings.AddGroup(TEXT("MouseXY"), INVTEXT("Sensitivity"));
		{
			GET_PROPERTY_VAR(Sensitivity, Sensitivity)
			{
				const TSharedRef<IPropertyHandle> X = Sensitivity->GetChildHandle(0).ToSharedRef();
				ADD_CLAMP_META(X, 0.1, 2.0)

				const TSharedRef<IPropertyHandle> Y = Sensitivity->GetChildHandle(1).ToSharedRef();
				ADD_CLAMP_META(Y, 0.1, 2.0)

				SensitivityGroup.HeaderRow().OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(Sensitivity))
				.NameContent()
				[
					Sensitivity->CreatePropertyNameWidget()
				]
				.ValueContent()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.Padding(0.0f, 0.0f, 10.0f, 0.0f)
					.AutoWidth()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.Padding(0.0f, 0.0f, 3.0f, 0.0f)
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							GENERIC_PROPERTY_LABEL(X)
						]
						+SHorizontalBox::Slot()
						.MinWidth(115.0f)
						.AutoWidth()
						[
							X->CreatePropertyValueWidget()
						]
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.Padding(0.0f, 0.0f, 3.0f, 0.0f)
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							GENERIC_PROPERTY_LABEL(Y)
						]
						+SHorizontalBox::Slot()
						.MinWidth(115.0f)
						.AutoWidth()
						[
							Y->CreatePropertyValueWidget()
						]
					]
				];
			}

			GET_PROPERTY_VAR(SensitivityMulti, Multi)

			const TSharedRef<IPropertyHandle> Base = Multi->GetChildHandle(0).ToSharedRef();
			ADD_CLAMP_META(Base, 0.1, 2.0)
			SensitivityGroup.AddPropertyRow(Base)
				.DisplayName(INVTEXT("Base Multi"));

			SensitivityGroup.AddPropertyRow(Multi->GetChildHandle(1).ToSharedRef())
				.DisplayName(INVTEXT("Multipliers"));
		}

		IDetailGroup& LeanGroup = Settings.AddGroup(TEXT("Leaning"), INVTEXT("Leaning"));
		{
			GET_PROPERTY_VAR(LeanOffsets, LeanOffsets)
			{
				const TSharedRef<IPropertyHandle> Pos = LeanOffsets->GetChildHandle(0).ToSharedRef();
				ADD_CLAMP_META_NO_UPPER(Pos, 5.0, 100.0)

				const TSharedRef<IPropertyHandle> Rot = LeanOffsets->GetChildHandle(1).ToSharedRef();
				ADD_CLAMP_META_NO_UPPER(Rot, 5.0, 100.0)

				LeanGroup.HeaderRow().OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(LeanOffsets))
				.NameContent()
				[
					GENERIC_PROPERTY_LABEL(Leaning)
				]
				.ValueContent()
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.Padding(0.0f, 2.0f, 0.0f, 2.0f)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.Padding(0.0f, 0.0f, 4.0f, 0.0f)
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							GENERIC_PROPERTY_LABEL(Position)
						]
						+SHorizontalBox::Slot()
						.MinWidth(82.0f)
						.AutoWidth()
						[
							Pos->CreatePropertyValueWidget()
						]
					]
					+SVerticalBox::Slot()
					.Padding(0.0f, 2.0f, 0.0f, 2.0f)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.Padding(0.0f, 0.0f, 3.0f, 0.0f)
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							GENERIC_PROPERTY_LABEL(Rotation)
						]
						+SHorizontalBox::Slot()
						.MinWidth(82.0f)
						.AutoWidth()
						[
							Rot->CreatePropertyValueWidget()
						]
					]
				];
			}

			LeanGroup.AddPropertyRow(GET_PROPERTY(LeanSpeed)).DisplayName(INVTEXT("Speed"));

			GET_PROPERTY_VAR(SideTrace, SideTrace)
			GET_PROPERTY_VAR(SideTraceLength, SideTraceLength)
			ADD_CLAMP_META_NO_UPPER(SideTraceLength, 10.0, 200.0)
			LeanGroup.AddWidgetRow().OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(SideTraceLength))
			.NameContent()
			[
				GENERIC_PROPERTY_LABEL(Side Trace)
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 10.0f, 0.0f)
				.MinWidth(125.0f)
				.AutoWidth()
				[
					SideTrace->CreatePropertyValueWidget()
				]
				+SHorizontalBox::Slot()
				.MinWidth(125.0f)
				.AutoWidth()
				[
					SideTraceLength->CreatePropertyValueWidget()
				]
			];
		}

		IDetailGroup& MoveGroup = Settings.AddGroup(TEXT("Movement"), INVTEXT("Movement"));
		{
			GET_PROPERTY_VAR(MoveSpeedMulti, SpeedMulti)

			const TSharedRef<IPropertyHandle> SpeedBase = SpeedMulti->GetChildHandle(0).ToSharedRef();
			ADD_CLAMP_META_NO_UPPER(SpeedBase, 0.1, 5.0)
			MoveGroup.HeaderRow()
			.OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(SpeedBase))
			.NameContent()
			[
				GENERIC_PROPERTY_LABEL(Movement)
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 4.0f, 0.0f)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					GENERIC_PROPERTY_LABEL(Speed)
				]
				+SHorizontalBox::Slot()
				.MinWidth(92.0f)
				.AutoWidth()
				[
					SpeedBase->CreatePropertyValueWidget()
				]
			];
			
			MoveGroup.AddPropertyRow(SpeedMulti->GetChildHandle(1).ToSharedRef())
				.DisplayName(INVTEXT("Multipliers"));

			MoveGroup.AddPropertyRow(GET_PROPERTY(WalkingSpeed)).DisplayName(INVTEXT("Walk Speed"));
			
			GET_PROPERTY_VAR(SwayOffsets, SwayOffsets)
			{
				const TSharedRef<IPropertyHandle> Pos = SwayOffsets->GetChildHandle(0).ToSharedRef();
				ADD_CLAMP_META_NO_UPPER(Pos, 0.0, 5.0)

				const TSharedRef<IPropertyHandle> Rot = SwayOffsets->GetChildHandle(1).ToSharedRef();
				ADD_CLAMP_META_NO_UPPER(Rot, 0.0, 5.0)

				MoveGroup.AddWidgetRow().OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(SwayOffsets))
				.NameContent()
				[
					SwayOffsets->CreatePropertyNameWidget()
				]
				.ValueContent()
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.Padding(0.0f, 2.0f, 0.0f, 2.0f)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.Padding(0.0f, 0.0f, 4.0f, 0.0f)
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							GENERIC_PROPERTY_LABEL(Position)
						]
						+SHorizontalBox::Slot()
						.MinWidth(82.0f)
						.AutoWidth()
						[
							Pos->CreatePropertyValueWidget()
						]
					]
					+SVerticalBox::Slot()
					.Padding(0.0f, 2.0f, 0.0f, 2.0f)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.Padding(0.0f, 0.0f, 3.0f, 0.0f)
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							GENERIC_PROPERTY_LABEL(Rotation)
						]
						+SHorizontalBox::Slot()
						.MinWidth(82.0f)
						.AutoWidth()
						[
							Rot->CreatePropertyValueWidget()
						]
					]
				];
			}

			IDetailGroup& CrouchGroup = MoveGroup.AddGroup(TEXT("Crouching"), INVTEXT("Crouching"));
			{
				GET_PROPERTY_VAR(CrouchWalkSpeed, CrouchWalkSpeed)

				CrouchGroup.HeaderRow()
				.OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(CrouchWalkSpeed))
				.NameContent()
				[
					GENERIC_PROPERTY_LABEL(Crouching)
				]
				.ValueContent()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.Padding(0.0f, 0.0f, 4.0f, 0.0f)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						GENERIC_PROPERTY_LABEL(Speed)
					]
					+SHorizontalBox::Slot()
					.MinWidth(92.0f)
					.AutoWidth()
					[
						CrouchWalkSpeed->CreatePropertyValueWidget()
					]
				];

				CrouchGroup.AddPropertyRow(GET_PROPERTY(CrouchFOV)).DisplayName(INVTEXT("FOV Offset"));
				CrouchGroup.AddPropertyRow(GET_PROPERTY(CeilingTrace));

				GET_PROPERTY_VAR(CrouchHeights, CrouchHeights)
				{
					const TSharedRef<IPropertyHandle> Stand = CrouchHeights->GetChildHandle(0).ToSharedRef();
					ADD_CLAMP_META_NO_UPPER(Stand, 10.0, 100.0)

					const TSharedRef<IPropertyHandle> Crouch = CrouchHeights->GetChildHandle(1).ToSharedRef();
					ADD_CLAMP_META_NO_UPPER(Crouch, 10.0, 100.0)

					CrouchGroup.AddWidgetRow().OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(CrouchHeights))
					.NameContent()
					[
						GENERIC_PROPERTY_LABEL(Heights)
					]
					.ValueContent()
					[
						SNew(SVerticalBox)
						+SVerticalBox::Slot()
						.Padding(0.0f, 2.0f, 0.0f, 2.0f)
						.AutoHeight()
						[
							SNew(SHorizontalBox)
							+SHorizontalBox::Slot()
							.Padding(0.0f, 0.0f, 9.0f, 0.0f)
							.VAlign(VAlign_Center)
							.AutoWidth()
							[
								GENERIC_PROPERTY_LABEL(Stand)
							]
							+SHorizontalBox::Slot()
							.MinWidth(88.0f)
							.AutoWidth()
							[
								Stand->CreatePropertyValueWidget()
							]
						]
						+SVerticalBox::Slot()
						.Padding(0.0f, 2.0f, 0.0f, 2.0f)
						.AutoHeight()
						[
							SNew(SHorizontalBox)
							+SHorizontalBox::Slot()
							.Padding(0.0f, 0.0f, 4.0f, 0.0f)
							.VAlign(VAlign_Center)
							.AutoWidth()
							[
								GENERIC_PROPERTY_LABEL(Crouch)
							]
							+SHorizontalBox::Slot()
							.MinWidth(88.0f)
							.AutoWidth()
							[
								Crouch->CreatePropertyValueWidget()
							]
						]
					];
				}

				CrouchGroup.AddPropertyRow(GET_PROPERTY(CrouchRate)).DisplayName(INVTEXT("Anim Speed"));
				CrouchGroup.AddPropertyRow(GET_PROPERTY(CrouchAnim)).DisplayName(INVTEXT("Animation"));
			}

			IDetailGroup& RunGroup = MoveGroup.AddGroup(TEXT("Running"), INVTEXT("Running"));
			{
				GET_PROPERTY_VAR(RunningSpeed, RunningSpeed)

				RunGroup.HeaderRow()
				.OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(RunningSpeed))
				.NameContent()
				[
					GENERIC_PROPERTY_LABEL(Running)
				]
				.ValueContent()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.Padding(0.0f, 0.0f, 4.0f, 0.0f)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						GENERIC_PROPERTY_LABEL(Speed)
					]
					+SHorizontalBox::Slot()
					.MinWidth(92.0f)
					.AutoWidth()
					[
						RunningSpeed->CreatePropertyValueWidget()
					]
				];

				RunGroup.AddPropertyRow(GET_PROPERTY(RunningFOV)).DisplayName(INVTEXT("FOV Offset"));
				
				RunGroup.AddPropertyRow(GET_PROPERTY(MaxStamina));
				RunGroup.AddPropertyRow(GET_PROPERTY(StaminaDrainRate)).DisplayName(INVTEXT("Drain Rate"));
				RunGroup.AddPropertyRow(GET_PROPERTY(StaminaGainRate)).DisplayName(INVTEXT("Gain Rate"));
			}
		}
	}
};
#undef CLASSNAME