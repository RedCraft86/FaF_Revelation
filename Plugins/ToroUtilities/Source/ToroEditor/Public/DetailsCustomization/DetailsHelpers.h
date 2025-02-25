// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailLayoutBuilder.h"
#include "PropertyCustomizationHelpers.h"

#define GET_MEMBER(Class, Member) GET_MEMBER_NAME_CHECKED(Class, Member)

#define ADD_META(Property, Key, Value) Property->SetInstanceMetaData(TEXT(#Key), TEXT(#Value)); \

#define ADD_CLAMP_META(Property, Min, Max) \
	ADD_META(Property, ClampMin, Min) \
	ADD_META(Property, UIMin, Min) \
	ADD_META(Property, ClampMax, Max) \
	ADD_META(Property, UIMax, Max)

#define GET_CLASS_PROPERTY(Class, Member) DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(Class, Member))
#define GET_PROPERTY(Member) GET_CLASS_PROPERTY(CLASSNAME, Member)

#define GET_CLASS_PROPERTY_VAR(Class, Member, VarName) \
	TSharedRef<IPropertyHandle> VarName = GET_CLASS_PROPERTY(Class, Member); \
	VarName->MarkHiddenByCustomization();

#define GET_PROPERTY_VAR(Member, VarName) GET_CLASS_PROPERTY_VAR(CLASSNAME, Member, VarName)

#define STRUCT_CLASS_PROPERTY(Struct, Member) StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(Struct, Member)).ToSharedRef()
#define STRUCT_PROPERTY(Member) STRUCT_CLASS_PROPERTY(STRUCTNAME, Member)

#define STRUCT_CLASS_PROPERTY_VAR(Struct, Member, VarName) \
	TSharedPtr<IPropertyHandle> VarName = STRUCT_CLASS_PROPERTY(Struct, Member); \
	VarName->MarkHiddenByCustomization();

#define STRUCT_PROPERTY_VAR(Member, VarName) STRUCT_CLASS_PROPERTY_VAR(STRUCTNAME, Member, VarName)

#define SIMPLE_RESET_TO_DEFAULT(Property) FResetToDefaultOverride::Create( \
	TAttribute<bool>::CreateLambda([Property]() -> bool { \
		return Property->DiffersFromDefault(); \
	}), FSimpleDelegate::CreateLambda([Property]() { \
		Property->ResetToDefault(); \
	}))

#define GENERIC_PROPERTY_LABEL(InText) \
	SNew(STextBlock) \
	.Text(INVTEXT(#InText)) \
	.Font(IDetailLayoutBuilder::GetDetailFont())

namespace DetailsHelpers
{
	TOROEDITOR_API inline bool IsPropertyFromChildClass(const TSharedRef<IPropertyHandle>& InHandle, const UClass* BaseClass)
	{
		if (!BaseClass) return false;
		const FProperty* Property = InHandle->GetProperty();
		const UClass* OwningClass = Property ? Property->GetOwnerClass() : nullptr;
		return OwningClass ? BaseClass->IsChildOf(OwningClass) : false;
	}
}