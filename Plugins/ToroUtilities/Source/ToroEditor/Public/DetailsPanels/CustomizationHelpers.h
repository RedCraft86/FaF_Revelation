// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailLayoutBuilder.h"
#include "PropertyCustomizationHelpers.h"

#define GET_MEMBER(Class, Member) GET_MEMBER_NAME_CHECKED(Class, Member)

#define ADD_META(Property, Key, Value) Property->SetInstanceMetaData(TEXT(#Key), TEXT(#Value)); \

#define CLASS_PROPERTY(Class, Member) DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(Class, Member))
#define CLASS_PROPERTY_NS(Member) CLASS_PROPERTY(CLASSNAME, Member)

#define CLASS_PROPERTY_VAR(Class, Member, VarName) \
	TSharedRef<IPropertyHandle> VarName = CLASS_PROPERTY(Class, Member); \
	VarName->MarkHiddenByCustomization();
#define CLASS_PROPERTY_VAR_NS(Member, VarName) CLASS_PROPERTY_VAR(CLASSNAME, Member, VarName)

#define STRUCT_PROPERTY(Struct, Member) StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(Struct, Member)).ToSharedRef()
#define STRUCT_PROPERTY_NS(Member) STRUCT_PROPERTY(STRUCTNAME, Member)

#define STRUCT_PROPERTY_VAR(Struct, Member, VarName) \
	TSharedPtr<IPropertyHandle> VarName = STRUCT_PROPERTY(Struct, Member); \
	VarName->MarkHiddenByCustomization();
#define STRUCT_PROPERTY_VAR_NS(Member, VarName) STRUCT_PROPERTY_VAR(STRUCTNAME, Member, VarName)

#define SIMPLE_RESET_TO_DEFAULT(Property) FResetToDefaultOverride::Create( \
	TAttribute<bool>::CreateLambda([Property]() -> bool { \
		return Property->DiffersFromDefault(); \
	}), FSimpleDelegate::CreateLambda([Property]() { \
		Property->ResetToDefault(); \
	}))

#define GENERIC_LABEL(InText) \
	SNew(STextBlock) \
	.Text(INVTEXT(#InText)) \
	.Font(IDetailLayoutBuilder::GetDetailFont())

namespace CustomizationHelpers
{
	TOROEDITOR_API inline bool IsPropertyFromChildClass(const TSharedRef<IPropertyHandle>& InHandle, const UClass* BaseClass)
	{
		if (!BaseClass) return false;
		const FProperty* Property = InHandle->GetProperty();
		const UClass* OwningClass = Property ? Property->GetOwnerClass() : nullptr;
		return OwningClass ? BaseClass->IsChildOf(OwningClass) : false;
	}
}