// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroTextureUtils.h"
#include "Slate/WidgetRenderer.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Blueprint/UserWidget.h"
#include "ImageUtils.h"

void UToroTextureUtils::DrawWidgetToRenderTarget(UTextureRenderTarget2D* InRenderTarget, UUserWidget* InUserWidget)
{
	if (!IsValid(InRenderTarget) || !IsValid(InUserWidget)) return;
	
	const TSharedRef<SWidget> Widget = InUserWidget->TakeWidget();
	FWidgetRenderer* Renderer = new FWidgetRenderer(true, false);
	Renderer->DrawWidget(InRenderTarget, Widget, Widget->GetCachedGeometry().Scale,
		FVector2D(InRenderTarget->SizeX, InRenderTarget->SizeY), 0.0f);
}

FToroTextureData UToroTextureUtils::GetDataFromRenderTarget(UTextureRenderTarget2D* InRenderTarget, const bool bInvertAlpha)
{
	if (!IsValid(InRenderTarget)) return {};
	
	FToroTextureData Result;
	InRenderTarget->GameThread_GetRenderTargetResource()->ReadPixels(Result.Pixels);
	Result.Size = {InRenderTarget->SizeX, InRenderTarget->SizeY};
	if (bInvertAlpha)
	{
		for (FColor& Pixel : Result.Pixels)
		{
			Pixel.A = (uint8)((int32)Pixel.A * -1 + 255);
		}
	}
	return Result;
}

FToroTextureData UToroTextureUtils::GetDataFromTexture(const UTexture2D* InTexture)
{
	if (!IsValid(InTexture)) return {};

	FToroTextureData Result;
	const FTexturePlatformData* PlatformData = InTexture->GetPlatformData();
	Result.Size = {PlatformData->SizeX, PlatformData->SizeY};
	Result.Pixels.AddDefaulted(Result.Size.X * Result.Size.Y);
	const void* TexData = PlatformData->Mips[0].BulkData.LockReadOnly();
	FMemory::Memcpy(Result.Pixels.GetData(), TexData, sizeof(FColor) * Result.Pixels.Num());
	PlatformData->Mips[0].BulkData.Unlock();
	return Result;
}

UTexture2D* UToroTextureUtils::CreateTextureFromData(const FToroTextureData& InData)
{
	if (!InData.HasValidData()) return nullptr;
	
	UTexture2D* Image = UTexture2D::CreateTransient(InData.Size.X, InData.Size.Y, PF_B8G8R8A8);
	FTexturePlatformData* PlatformData = Image->GetPlatformData();
	void* TextureData = PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, InData.Pixels.GetData(), sizeof(FColor) * InData.Size.X * InData.Size.Y);
	PlatformData->Mips[0].BulkData.Unlock();
	Image->UpdateResource();
	
	return Image;
}

UTexture2D* UToroTextureUtils::ConvertRenderTargetToTexture(UTextureRenderTarget2D* InRenderTarget, const bool bHasAlpha)
{
	return CreateTextureFromData(GetDataFromRenderTarget(InRenderTarget, bHasAlpha));
}

void UToroTextureUtils::SaveTextureDataToFile(const FToroTextureData& InData, const FString& InPath, const bool bAsync, const FString& FileExtension)
{
	if (InPath.IsEmpty() || !InData.HasValidData()) return;
	AsyncTask(bAsync ? ENamedThreads::AnyBackgroundHiPriTask : ENamedThreads::GameThread, [InData, InPath, FileExtension]() {
		TArray64<uint8> ImageBinary;
		FImageUtils::PNGCompressImageArray(InData.Size.X, InData.Size.Y, InData.Pixels, ImageBinary);
		FFileHelper::SaveArrayToFile(ImageBinary, *(InPath + (FileExtension.IsEmpty() ? TEXT(".png") : FileExtension)));
	});
}

void UToroTextureUtils::SaveTextureToFile(const UTexture2D* InTexture, const FString& InPath, const bool bAsync, const FString& FileExtension)
{
	if (InPath.IsEmpty() || !IsValid(InTexture)) return;
	const FToroTextureData Data = GetDataFromTexture(InTexture);
	SaveTextureDataToFile(Data, InPath, bAsync, FileExtension);
}

void UToroTextureUtils::SaveRenderTargetToFile(UTextureRenderTarget2D* InRenderTarget, const FString& InPath, const bool bHasAlpha, const bool bAsync)
{
	if (InPath.IsEmpty() || !IsValid(InRenderTarget)) return;
	const FToroTextureData Data = GetDataFromRenderTarget(InRenderTarget, bHasAlpha);
	SaveTextureDataToFile(Data, InPath, bAsync);
}