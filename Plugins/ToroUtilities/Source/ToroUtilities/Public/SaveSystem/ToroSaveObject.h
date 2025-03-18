// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCore.h"
#include "UObject/Object.h"
#include "ToroSaveObject.generated.h"

class UToroSaveManager;

UE_DECLARE_GAMEPLAY_TAG_BASE(Save)
UE_DECLARE_GAMEPLAY_TAG_CHILD(Global, Save)
UE_DECLARE_GAMEPLAY_TAG_CHILD(Game, Save)

UENUM(BlueprintInternalUseOnly)
enum class ESaveGameError : uint8
{
	None,
	Compress,
	Decompress,
	FileWrite,
	FileRead,
};

UCLASS(Abstract)
class TOROUTILITIES_API UToroSaveObject : public UObject
{
	GENERATED_BODY()

public:

	UToroSaveObject() : LastError(ESaveGameError::None) {}
	static UToroSaveObject* Create(UToroSaveManager* Owner, const TSubclassOf<UToroSaveObject>& Class, const FGameplayTag& Tag);

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
	UPROPERTY(Transient) TObjectPtr<UToroSaveManager> SaveManager;

	virtual void SerializeData(FArchive& Ar) {}
};
