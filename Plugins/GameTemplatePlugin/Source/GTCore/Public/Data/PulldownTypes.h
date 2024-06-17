﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PulldownTypes.generated.h"

USTRUCT()
struct GTCORE_API FPulldownOption final
{
	GENERATED_BODY()
#if WITH_EDITORONLY_DATA
	UPROPERTY() FString Option;
	UPROPERTY() FString Tooltip;
	FPulldownOption() : Option(TEXT("")), Tooltip(TEXT("")) {}
	FPulldownOption(const FString& InOption, const FString& InTooltip)
		: Option(InOption), Tooltip(InTooltip) {}

	FORCEINLINE bool operator==(const FPulldownOption& Other) const
	{
		return Option.Equals(Other.Option);
	}

	FORCEINLINE bool operator!=(const FPulldownOption& Other) const
	{
		return !(*this == Other);
	}
#else
	FPulldownOption() {}
#endif
};

USTRUCT(BlueprintInternalUseOnly)
struct GTCORE_API FStringPulldown
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pulldown")
		FString Value;

	FStringPulldown() : Value(TEXT("")) {}
	virtual ~FStringPulldown() = default;
	FORCEINLINE FString operator*() const { return Value; }
	bool IsEmpty() const { return Value.IsEmpty(); }

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, meta = (TransientToolProperty)) bool bAllowCustomInput = true;
	virtual TArray<FPulldownOption> GetPulldownOptions() { return {}; }
#endif
};

USTRUCT(BlueprintInternalUseOnly)
struct GTCORE_API FStringListPulldown : public FStringPulldown
{
	GENERATED_BODY()
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, meta = (TransientToolProperty)) TArray<FPulldownOption> Options = {};
	virtual TArray<FPulldownOption> GetPulldownOptions() override { return Options; }
#endif
};