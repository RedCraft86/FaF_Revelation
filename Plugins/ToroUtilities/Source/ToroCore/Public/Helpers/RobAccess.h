﻿// Copyright (C) RedCraft86. All Rights Reserved.

// Rob is possible thanks to: http://bloglitb.blogspot.com/2011/12/access-to-private-members-safer.html

#pragma once

template<typename Tag, typename Tag::Type M>
struct Rob
{
	friend typename Tag::Type Access(Tag)
	{
		return M;
	}
};

#define ROB_DEFINE_VAR(_Class, _VarName, _VarType) \
	struct F##_Class##_VarName##Tag \
	{ \
		using Type = _VarType _Class::*; \
		friend Type Access(F##_Class##_VarName##Tag); \
	}; \
	template struct Rob<F##_Class##_VarName##Tag, &_Class::_VarName>

#define ROB_DEFINE_FUNC(_Class, _FunName, _RetType, ...) \
	struct F##_Class##_FunName##Tag \
	{ \
		using Type = _RetType (_Class::*)(__VA_ARGS__); \
		friend Type Access(F##_Class##_FunName##Tag); \
	}; \
	template struct Rob<F##_Class##_FunName##Tag, &_Class::_FunName>

#define ROB_DEFINE_FUNC_CONST(_Class, _FunName, _RetType, ...) \
	struct F##_Class##_FunName##Tag \
	{ \
		using Type = _RetType (_Class::*)(__VA_ARGS__) const; \
		friend Type Access(F##_Class##_FunName##Tag); \
	}; \
	template struct Rob<F##_Class##_FunName##Tag, &_Class::_FunName>

#define RobAccess(_Class, _VarName) Access(F##_Class##_VarName##Tag())