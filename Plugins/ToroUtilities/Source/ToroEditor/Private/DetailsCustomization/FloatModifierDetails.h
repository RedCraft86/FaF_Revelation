// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsCustomization/DetailsHelpers.h"
#include "DataTypes/AdvancedFloatTypes.h"

#define STRUCTNAME FToroFloatModifier
class FFloatModifierCustomization final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FFloatModifierCustomization>();
	}

private:

	UScriptStruct* ScriptStruct = nullptr;

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		if (FProperty* Property = StructPropertyHandle->GetProperty())
		{
			if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
			{
				ScriptStruct = StructProperty->Struct;
				if (ScriptStruct)
				{
					STRUCT_PROPERTY_VAR(Base, Base)
					HeaderRow.OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(Base))
					.NameContent()
					[
						StructPropertyHandle->CreatePropertyNameWidget()
					]
					.ValueContent()
					[
						Base->CreatePropertyValueWidget()
					];

					return;
				}
			}
		}

		HeaderRow.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			GENERIC_PROPERTY_LABEL(Error)
		];
	}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		if (ScriptStruct)
		{
			FString ModifierType = ScriptStruct->GetMetaData(TEXT("ModifierType"));
			if (ModifierType.IsEmpty()) ModifierType = TEXT("Modifiers");
			UE_LOG(LogTemp, Warning, TEXT("TypeName: %s"), *ModifierType);
			StructBuilder.AddProperty(STRUCT_PROPERTY(Modifiers))
				.DisplayName(FText::FromString(ModifierType));
		}
	}
};
#undef STRUCTNAME