// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#define GET_CLASS_PROPERTY(Class, Member) DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(Class, Member))

#define GET_CLASS_PROPERTY_VAR(Class, Member, VarName) \
	TSharedRef<IPropertyHandle> VarName = GET_CLASS_PROPERTY(Class, Member); \
	VarName->MarkHiddenByCustomization();