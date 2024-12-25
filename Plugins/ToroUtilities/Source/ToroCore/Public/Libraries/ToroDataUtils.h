// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroDataUtils.generated.h"

UCLASS(DisplayName = "Data Utils")
class TOROCORE_API UToroDataUtils final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "DataUtils")
		static FString EncryptAES(const FString& String, const FString& Key);

	UFUNCTION(BlueprintCallable, Category = "DataUtils")
		static FString DecryptAES(const FString& String, const FString& Key);

	UFUNCTION(BlueprintCallable, Category = "DataUtils")
		static TArray<uint8> CompressBytes(const TArray<uint8>& InBytes);

	UFUNCTION(BlueprintCallable, Category = "DataUtils")
		static TArray<uint8> DecompressBytes(const TArray<uint8>& InBytes);
};
