// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/MiscTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"

UCLASS(DisplayName = "Texture Utils")
class TOROCORE_API UToroTextureUtils final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static void DrawWidgetToRenderTarget(UTextureRenderTarget2D* InRenderTarget, UUserWidget* InUserWidget);
	
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static FToroTextureData GetDataFromRenderTarget(UTextureRenderTarget2D* InRenderTarget, const bool bInvertAlpha);

	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static FToroTextureData GetDataFromTexture(const UTexture2D* InTexture);

	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static UTexture2D* CreateTextureFromData(const FToroTextureData& InData);
	
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static UTexture2D* ConvertRenderTargetToTexture(UTextureRenderTarget2D* InRenderTarget, const bool bHasAlpha);
	
	UFUNCTION(BlueprintCallable, Category = TextureUtils, meta = (AdvancedDisplay = "FileExtension"))
		static void SaveTextureDataToFile(const FToroTextureData& InData, const FString& InPath, const bool bAsync = true, const FString& FileExtension = TEXT(""));
	
	UFUNCTION(BlueprintCallable, Category = TextureUtils, meta = (AdvancedDisplay = "FileExtension"))
		static void SaveTextureToFile(const UTexture2D* InTexture, const FString& InPath, const bool bAsync = true, const FString& FileExtension = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static void SaveRenderTargetToFile(UTextureRenderTarget2D* InRenderTarget, const FString& InPath, const bool bHasAlpha, const bool bAsync = true);
};