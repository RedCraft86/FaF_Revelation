// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsPanels/CustomizationHelpers.h"
#include "DataTypes/MathTypes.h"

#define STRUCTNAME FToroFloatModifier
class FFloatModifierDetails final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FFloatModifierDetails>();
	}

private:

	TObjectPtr<UScriptStruct> ScriptStruct = nullptr;
	static inline TSet<FName> ClampMetadata = { TEXT("ClampMin"), TEXT("ClampMax"), TEXT("UIMin"), TEXT("UIMax") };

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		FProperty* Property = StructPropertyHandle->GetProperty();
		const FStructProperty* StructProperty = Property ? CastField<FStructProperty>(Property) : nullptr;
		ScriptStruct = StructProperty ? StructProperty->Struct : nullptr;
		if (ScriptStruct)
		{
			STRUCT_PROPERTY_VAR_NS(Base, Base)
			for (const FName& Meta : ClampMetadata)
			{
				if (FString Value = Property->GetMetaData(Meta); !Value.IsEmpty())
				{
					Base->SetInstanceMetaData(Meta, Value);
				}
			}

			HeaderRow.OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(Base))
			.NameContent()
			[
				StructPropertyHandle->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				Base->CreatePropertyValueWidget()
			];
		}
		else
		{
			HeaderRow.NameContent()
			[
				StructPropertyHandle->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				GENERIC_LABEL(Error)
			];
		}
	}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		if (ScriptStruct)
		{
			FString ModifierType = ScriptStruct->GetMetaData(TEXT("ModifierType"));
			if (ModifierType.IsEmpty()) ModifierType = TEXT("Modifiers");

			StructBuilder.AddProperty(STRUCT_PROPERTY_NS(Modifiers)).DisplayName(FText::FromString(ModifierType));
		}
	}
};
#undef STRUCTNAME