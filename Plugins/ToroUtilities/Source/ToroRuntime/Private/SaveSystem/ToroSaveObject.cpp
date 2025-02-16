// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveObject.h"
#include "Serialization/BufferArchive.h"
#include "ToroRuntimeSettings.h"
#include "CompressionHelpers.h"
#include "ToroRuntime.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_Saves, "Saves");

FString GetDemoStr()
{
	FString Str = UToroRuntimeSettings::Get()->DemoVersion.ToString();
	return Str.IsEmpty() || Str == TEXT(" ") || Str.TrimStartAndEnd().IsEmpty() ? TEXT("") : Str;
}

UToroSaveObject* UToroSaveObject::Create(UObject* Owner, const TSubclassOf<UToroSaveObject>& Class, const FGameplayTag& Tag)
{
	if (Owner && Class && Tag.IsValid() && Tag != Tag_Saves)
	{
		FString L, Name;
		Tag.ToString().Split(TEXT("."), &L, &Name, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		if (UToroSaveObject* NewObj = NewObject<UToroSaveObject>(Owner, Class, *Name))
		{
			NewObj->SaveTag = Tag;
			NewObj->SavePath = FPaths::MakeValidFileName(FString::Printf(TEXT("%sSaveGames/%s/%s.tsave"),
				*FPaths::ProjectSavedDir(), *GetDemoStr(), *Name).Replace(TEXT("//"), TEXT("/")));
		
			NewObj->LoadObject(nullptr);
			UE_LOG(LogTemp, Warning, TEXT("Created Save Slot For: %s"), *NewObj->SavePath);
			return NewObj;
		}
	}
	
	return nullptr;
}

void UToroSaveObject::DeleteData()
{
	IFileManager::Get().Delete(*SavePath);
	LastError = ESaveGameError::None;
}

void UToroSaveObject::SaveObject(const TFunction<void(const ESaveGameError)>& Callback)
{
	FBufferArchive ToBinary(true);
	SerializeData(ToBinary);

	if (!ToBinary.IsEmpty())
	{
		TArray<uint8> CompressedData;
		if (CompressionHelpers::OodleCompress(ToBinary, CompressedData))
		{
			AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this, Callback, CompressedData]()
			{
				LastError = FFileHelper::SaveArrayToFile(CompressedData, *SavePath)
					? ESaveGameError::None : ESaveGameError::FileWrite;
	#if WITH_EDITOR
				if (LastError == ESaveGameError::FileWrite)
				{
					UE_LOG(LogToroRuntime, Error, TEXT("Failed write save file! [Path: %s] [Size: %d]"),
						*SavePath, CompressedData.Num());
				}
	#endif
				if (Callback) Callback(LastError);
			});
		}
		else
		{
			UE_LOG(LogToroRuntime, Error, TEXT("Failed to compress save data! [Size: %d]"), ToBinary.Num());
			LastError = ESaveGameError::Compress;
			if (Callback) Callback(LastError);
		}
	}
	else
	{
		LastError = ESaveGameError::None;
		if (Callback) Callback(LastError);
	}

	ToBinary.FlushCache();
	ToBinary.Close();
}

void UToroSaveObject::LoadObject(const TFunction<void(const ESaveGameError)>& Callback)
{
	if (!FPaths::FileExists(SavePath))
	{
		SaveObject(Callback);
		return;
	}

	AsyncTask(ENamedThreads::Type::AnyBackgroundHiPriTask, [this, Callback]()
	{
		if (TArray<uint8> CompressedData; FFileHelper::LoadFileToArray(CompressedData, *SavePath))
		{
			if (!CompressedData.IsEmpty())
			{
				AsyncTask(ENamedThreads::Type::GameThread, [this, Callback, CompressedData]()
				{
					TArray<uint8> DecompressedData;
					if (CompressionHelpers::OodleDecompress(CompressedData, DecompressedData))
					{
						FMemoryReader FromBinary(DecompressedData, true);
						FromBinary.Seek(0);

						SerializeData(FromBinary);

						DecompressedData.Empty();
						FromBinary.FlushCache();
						FromBinary.Close();
						
						LastError = ESaveGameError::None;
					}
					else
					{
						UE_LOG(LogToroRuntime, Error, TEXT("Failed to decompress save data! [Size: %d]"), CompressedData.Num());
						LastError = ESaveGameError::Decompress;
					}
					if (Callback) Callback(LastError);
				});

			   CompressedData.Empty();
			}
			else
			{
				LastError = ESaveGameError::None;
				if (Callback) Callback(LastError);
			}
		}
		else
		{
			UE_LOG(LogToroRuntime, Error, TEXT("Failed to read save file! [Path: %s]"), *SavePath);
			LastError = ESaveGameError::FileRead;
			if (Callback) Callback(LastError);
		}
	});
}
