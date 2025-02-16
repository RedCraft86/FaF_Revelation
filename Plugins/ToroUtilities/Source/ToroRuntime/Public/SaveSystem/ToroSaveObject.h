// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "ToroSaveObject.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Saves);

UENUM(BlueprintInternalUseOnly)
enum class ESaveGameError : uint8
{
	None,
	Compress,
	Decompress,
	FileWrite,
	FileRead,
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FSaveObjectInfo
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = SaveInfo, meta = (Categories = "Saves"))
		FGameplayTag Tag;

	UPROPERTY(Config, EditAnywhere, Category = SaveInfo, meta = (ClampMax = 99, UIMax = 99))
		uint8 Slot;

	UPROPERTY(Config, EditAnywhere, Category = SaveInfo)
		TSoftClassPtr<class UToroSaveObject> Class;

	FSaveObjectInfo() : Slot(0) {}
	bool IsValidData() const
	{
		return Tag.IsValid() && Tag != Tag_Saves && Slot <= 99 && (Class.IsValid() || !Class.IsNull());
	}
};

UCLASS(Abstract, BlueprintType, NotBlueprintable)
class TORORUNTIME_API UToroSaveObject : public UObject
{
	GENERATED_BODY()

public:

	UToroSaveObject() : LastError(ESaveGameError::None) {}

	static UToroSaveObject* Create(UObject* Owner, const TSubclassOf<UToroSaveObject>& Class, const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = SaveObject)
		ESaveGameError GetLastError() const { return LastError; }
	
	UFUNCTION(BlueprintPure, Category = SaveObject)
		FGameplayTag GetSaveTag() const { return SaveTag; }
	
	virtual void DeleteData();
	virtual void OnCreation() {}
	virtual void SaveObject(const TFunction<void(const ESaveGameError)>& Callback);
	virtual void LoadObject(const TFunction<void(const ESaveGameError)>& Callback);

protected:

	UPROPERTY() FString SavePath;
	UPROPERTY() FGameplayTag SaveTag;
	UPROPERTY() ESaveGameError LastError;
	virtual void SerializeData(FArchive& Ar) {}
};
