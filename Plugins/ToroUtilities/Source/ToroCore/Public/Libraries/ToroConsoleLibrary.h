// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroConsoleLibrary.generated.h"

#define SetCVarValue(Name, Value) if (IConsoleVariable* CVar = FindCVar(*Name)) { CVar->Set(Value); }
#define GetCVarValue(Name, Type, Default) \
	IConsoleVariable* CVar = FindCVar(*Name); \
	return CVar ? CVar->Get##Type() : Default;

UCLASS(DisplayName = "Console Library")
class TOROCORE_API UToroConsoleLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (Bool)")
		static void SetCVarBool(const FString& InName, const bool InValue) { SetCVarValue(InName, InValue) }

	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (Int)")
		static void SetCVarInt(const FString& InName, const int32 InValue) { SetCVarValue(InName, InValue) }

	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (float)")
		static void SetCVarFloat(const FString& InName, const float InValue) { SetCVarValue(InName, InValue) }

	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (String)")
		static void SetCVarString(const FString& InName, const FString& InValue) { SetCVarValue(InName, *InValue) }
	
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (Bool)")
		static bool GetCVarBool(const FString& InName) { GetCVarValue(InName, Bool, false) }

	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (Int)")
		static int32 GetCVarInt(const FString& InName) { GetCVarValue(InName, Int, 0) }

	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (float)")
		static float GetCVarFloat(const FString& InName) { GetCVarValue(InName, Float, 0.0f) }

	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (String)")
		static FString GetCVarString(const FString& InName) { GetCVarValue(InName, String, TEXT("")) }
	
	static IConsoleVariable* FindCVar(const FString& InName)
	{
		if (IConsoleVariable** CVar = CachedCVars.Find(InName); CVar && *CVar) { return *CVar; }
		if (IConsoleVariable* NewCVar = IConsoleManager::Get().FindConsoleVariable(*InName))
		{
			CachedCVars.Add(InName, NewCVar);
			return NewCVar;
		}
		return nullptr;
	}
	
private:
	
	static inline TMap<FString, IConsoleVariable*> CachedCVars = {};
};