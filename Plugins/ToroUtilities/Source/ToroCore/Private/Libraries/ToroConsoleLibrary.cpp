// Copyright (C) RedCraft86. All Rights Reserved.

#include "Libraries/ToroConsoleLibrary.h"

TMap<FString, IConsoleVariable*> UToroConsoleLibrary::CachedCVars = {};

void UToroConsoleLibrary::SetConsoleVariableBool(const FString& InName, const bool InValue)
{
	if (IConsoleVariable* CVar = FindCVar(*InName))
	{
		CVar->Set(InValue);
	}
}

void UToroConsoleLibrary::SetConsoleVariableInt(const FString& InName, const int32 InValue)
{
	if (IConsoleVariable* CVar = FindCVar(*InName))
	{
		CVar->Set(InValue);
	}
}

void UToroConsoleLibrary::SetConsoleVariableFloat(const FString& InName, const float InValue)
{
	if (IConsoleVariable* CVar = FindCVar(*InName))
	{
		CVar->Set(InValue);
	}
}

void UToroConsoleLibrary::SetConsoleVariableString(const FString& InName, const FString& InValue)
{
	if (IConsoleVariable* CVar = FindCVar(*InName))
	{
		CVar->Set(*InValue);
	}
}

bool UToroConsoleLibrary::GetConsoleVariableBool(const FString& InName)
{
	if (const IConsoleVariable* CVar = FindCVar(*InName))
	{
		return CVar->GetBool();
	}

	return false;
}

int32 UToroConsoleLibrary::GetConsoleVariableInt(const FString& InName)
{
	if (const IConsoleVariable* CVar = FindCVar(*InName))
	{
		return CVar->GetInt();
	}

	return 0;
}

float UToroConsoleLibrary::GetConsoleVariableFloat(const FString& InName)
{
	if (const IConsoleVariable* CVar = FindCVar(*InName))
	{
		return CVar->GetFloat();
	}

	return 0.0f;
}

FString UToroConsoleLibrary::GetConsoleVariableString(const FString& InName)
{
	if (const IConsoleVariable* CVar = FindCVar(*InName))
	{
		return CVar->GetString();
	}

	return TEXT("");
}

IConsoleVariable* UToroConsoleLibrary::FindCVar(const FString& InName)
{
	if (IConsoleVariable** CVar = CachedCVars.Find(InName); CVar && *CVar)
	{
		return *CVar;
	}

	if (IConsoleVariable* NewCVar = IConsoleManager::Get().FindConsoleVariable(*InName))
	{
		CachedCVars.Add(InName, NewCVar);
		return NewCVar;
	}

	return nullptr;
}
