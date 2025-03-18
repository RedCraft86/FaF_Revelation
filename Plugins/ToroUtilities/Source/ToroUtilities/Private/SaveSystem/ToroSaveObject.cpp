// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveObject.h"
#include "SaveSystem/ToroSaveManager.h"
#include "Serialization/BufferArchive.h"
#include "CompressionHelpers.h"
#include "WindowsHelpers.h"
#include "ToroUtilities.h"
#include "ToroSettings.h"

UE_DEFINE_GAMEPLAY_TAG_BASE(Save)
UE_DEFINE_GAMEPLAY_TAG_CHILD(Global, Save)
UE_DEFINE_GAMEPLAY_TAG_CHILD(Game, Save)

UToroSaveObject* UToroSaveObject::Create(UToroSaveManager* Owner, const TSubclassOf<UToroSaveObject>& Class, const FGameplayTag& Tag)
{
	if (Owner && Class && VerifySaveTag(Tag))
	{
		FString L, Name;
		Tag.ToString().Split(TEXT("."), &L, &Name, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		if (UToroSaveObject* NewObj = NewObject<UToroSaveObject>(Owner, Class, *Name))
		{
			NewObj->SaveTag = Tag;
			NewObj->SaveManager = Owner;
			NewObj->SavePath = FString::Printf(TEXT("%sSaveGames/%s/%s.tsave"), *FPaths::ProjectSavedDir(),
				*UToroSettings::Get()->GetDemoName(), *Name).Replace(TEXT("//"), TEXT("/"));
			
			NewObj->OnCreation();
			NewObj->LoadObject(nullptr);
			UE_LOG(LogToroUtilities, Display, TEXT("Created Save Object for File: %s"), *NewObj->SavePath);
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
				if (LastError == ESaveGameError::FileWrite)
				{
					WindowsHelpers::OpenDialogue(TEXT("Save System Error"), FString::Printf(
						TEXT("Failed to write file\n%s"), *SavePath),
						EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error);
					
					UE_LOG(LogToroUtilities, Error, TEXT("Failed write save file! [Path: %s] [Size: %d]"),
						*SavePath, CompressedData.Num());
				}
				if (Callback) Callback(LastError);
			});
		}
		else
		{
			WindowsHelpers::OpenDialogue(TEXT("Save System Error"), FString::Printf(
				TEXT("Failed to compress data for save file\n%s"), *SavePath),
				EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error);
			
			UE_LOG(LogToroUtilities, Error, TEXT("Failed to compress save data! [Size: %d]"), ToBinary.Num());
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

	if (SaveManager) SaveManager->OnSaveObjectIO.Broadcast(this);
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
						WindowsHelpers::OpenDialogue(TEXT("Save System Error"), FString::Printf(
							TEXT("Failed to decompress data from save file\n%s"), *SavePath),
							EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error);
						
						UE_LOG(LogToroUtilities, Error, TEXT("Failed to decompress save data! [Size: %d]"), CompressedData.Num());
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
			WindowsHelpers::OpenDialogue(TEXT("Save System Error"), FString::Printf(
				TEXT("Failed to read save file\n%s"), *SavePath),
				EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error);
			
			UE_LOG(LogToroUtilities, Error, TEXT("Failed to read save file! [Path: %s]"), *SavePath);
			LastError = ESaveGameError::FileRead;
			if (Callback) Callback(LastError);
		}
	});

	if (SaveManager) SaveManager->OnSaveObjectIO.Broadcast(this);
}
