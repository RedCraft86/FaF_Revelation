// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveTypes.h"
#include "Serialization/BufferArchive.h"
#include "Helpers/DataCompression.h"
#include "Helpers/WindowsHelpers.h"
#include "ToroUtilities.h"

#define LOG_ERROR(Msg, Data) WindowsHelpers::OpenDialogue(TEXT("Save System Error"), \
	FString::Printf(TEXT("%s\n%s"), TEXT(Msg), *SavePath), EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error); \
	UE_LOG(LogToroUtilities, Error, TEXT("%s [Size: %d] [Path: %s]"), TEXT(Msg), Data.Num(), *SavePath)

namespace SaveTags
{
	DEFINE_GAMEPLAY_TAG(Save)
	DEFINE_GAMEPLAY_TAG_CHILD(Save, Global)
	DEFINE_GAMEPLAY_TAG_CHILD(Save, Game)
}

UToroSaveObject* UToroSaveObject::Create(UObject* Owner, const TSubclassOf<UToroSaveObject>& Class, const FGameplayTag& Tag)
{
	if (!Owner || !Class || !SaveTags::IsValidTag(Tag))
		return nullptr;

	FString SaveName;
	Tag.ToString().Split(TEXT("."), nullptr, &SaveName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	if (SaveName.IsEmpty())
		return nullptr;

	if (UToroSaveObject* Obj = NewObject<UToroSaveObject>(Owner, Class, *SaveName))
	{
		Obj->SaveTag = Tag;
		Obj->SavePath = FPaths::Combine(*FPaths::ProjectSavedDir(), TEXT("SaveGames"), SaveName + TEXT(".tsave"));

		Obj->OnCreation();
		Obj->LoadObject(nullptr);
		UE_LOG(LogToroUtilities, Display, TEXT("Created Save Object for %s"), *Obj->SavePath);
		return Obj;
	}

	return nullptr;
}

FString UToroSaveObject::GetSaveName() const
{
	FString SaveName;
	SaveTag.ToString().Split(TEXT("."), nullptr,
		&SaveName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	return SaveName;
}

void UToroSaveObject::DeleteData()
{
	OnSaveActivity.Broadcast(this, ESaveGameActivity::Deleting);
	IFileManager::Get().Delete(*SavePath);
	SaveStatus = ESaveGameStatus::None;
}

void UToroSaveObject::SaveObject(const TFunction<void(const ESaveGameStatus)>& Callback)
{
	OnSaveActivity.Broadcast(this, ESaveGameActivity::Saving);

	FBufferArchive ToBinary(true);
	SerializeData(ToBinary);

	if (TArray<uint8> CompressedData; DataCompression::OodleCompress(ToBinary, CompressedData))
	{
		AsyncTask(ENamedThreads::Type::AnyBackgroundHiPriTask,
			[this, Callback, CompressedData = MoveTemp(CompressedData)]()
		{
			SaveStatus = FFileHelper::SaveArrayToFile(CompressedData, *SavePath)
				? SaveStatus = ESaveGameStatus::Success : SaveStatus = ESaveGameStatus::FileWriteError;
			
			if (SaveStatus == ESaveGameStatus::FileWriteError)
			{
				LOG_ERROR("Failed to write save file!", CompressedData)
			}

			if (Callback) Callback(SaveStatus);
		});
	}
	else
	{
		SaveStatus = ESaveGameStatus::CompressError;
		LOG_ERROR("Failed to compress save data!", ToBinary)
		if (Callback) Callback(SaveStatus);
	}

	ToBinary.FlushCache();
	ToBinary.Close();
}

void UToroSaveObject::LoadObject(const TFunction<void(const ESaveGameStatus)>& Callback)
{
	if (!FPaths::FileExists(SavePath))
	{
		SaveObject(Callback);
		return;
	}

	OnSaveActivity.Broadcast(this, ESaveGameActivity::Loading);

	AsyncTask(ENamedThreads::Type::AnyBackgroundHiPriTask, [this, Callback]()
	{
		TArray<uint8> CompressedData;
		if (FFileHelper::LoadFileToArray(CompressedData, *SavePath))
		{
			// Assume this is not a loading error and there's just no data to load
			if (CompressedData.IsEmpty())
			{
				SaveStatus = ESaveGameStatus::Success;
				Callback(SaveStatus);
				return;
			}

			AsyncTask(ENamedThreads::Type::GameThread,
				[this, Callback, CompressedData = MoveTemp(CompressedData)]()
			{
				if (TArray<uint8> RawData; DataCompression::OodleDecompress(CompressedData, RawData))
				{
					FMemoryReader FromBinary(RawData, true);
					FromBinary.Seek(0);

					SerializeData(FromBinary);

					FromBinary.FlushCache();
					FromBinary.Close();

					SaveStatus = ESaveGameStatus::Success;
				}
				else
				{
					SaveStatus = ESaveGameStatus::DecompressError;
					LOG_ERROR("Failed to decompress save data!", RawData)
				}

				if (Callback) Callback(SaveStatus);
			});
		}
		else
		{
			SaveStatus = ESaveGameStatus::FileReadError;
			LOG_ERROR("Failed to read save file!", CompressedData);
			if (Callback) Callback(SaveStatus);
		}
	});
}
