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
				.MinWidth(120.0f)
				.AutoWidth()
				[
					InteractionTrace->CreatePropertyValueWidget()
				]
				+SHorizontalBox::Slot()
				.MinWidth(120.0f)
				.AutoWidth()
				[
					ReachDistance->CreatePropertyValueWidget()
				]
			];
		}

		IDetailGroup& SensitivityGroup = Settings.AddGroup(TEXT("MouseXY"), INVTEXT("MouseXY"));
		{
			GET_PROPERTY_VAR(Sensitivity, Sensitivity)

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
					.MinWidth(110.0f)
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
					.MinWidth(110.0f)
					.AutoWidth()
					[
						Y->CreatePropertyValueWidget()
					]
				]
			];

			GET_PROPERTY_VAR(SensitivityMulti, Multi)

			const TSharedRef<IPropertyHandle> Base = Multi->GetChildHandle(0).ToSharedRef();
			ADD_CLAMP_META(Base, 0.1, 2.0)
			SensitivityGroup.AddPropertyRow(Base)
				.DisplayName(INVTEXT("Base Multi"));

			SensitivityGroup.AddPropertyRow(Multi->GetChildHandle(1).ToSharedRef())
				.DisplayName(INVTEXT("Multipliers"));
		}

		IDetailGroup& FOVGroup = Settings.AddGroup(TEXT("FieldOfView"), INVTEXT("Field Of View"));
		{
			GET_PROPERTY_VAR(FieldOfView, FieldOfView)

			const TSharedRef<IPropertyHandle> Base = FieldOfView->GetChildHandle(0).ToSharedRef();
			ADD_CLAMP_META(Base, 60.0, 140.0)
			FOVGroup.HeaderProperty(Base)
				.DisplayName(INVTEXT("Field Of View"));

			GET_PROPERTY_VAR(FieldOfViewSpeed, FieldOfViewSpeed)
			FOVGroup.AddPropertyRow(FieldOfViewSpeed).DisplayName(INVTEXT("Speed"));
			FOVGroup.AddPropertyRow(FieldOfView->GetChildHandle(1).ToSharedRef()).DisplayName(INVTEXT("Offsets"));
		}
	}
};
#undef CLASSNAME