// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "Helpers/GameplayTagMacros.h"
#include "ToroSaveTypes.generated.h"

namespace SaveTags
{
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(Save)
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(Global)
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(Game)

	TOROUTILITIES_API CREATE_TAG_VERIFIER(Save)
}

UENUM(BlueprintInternalUseOnly)
enum class ESaveGameStatus : uint8
{
	None,
	Success,
	CompressError,
	DecompressError,
	FileWriteError,
	FileReadError
};

UENUM(BlueprintInternalUseOnly)
enum class ESaveGameActivity : uint8
{
	Saving,
	Loading,
	Deleting
};

UCLASS(Abstract, NotBlueprintable, BlueprintType)
class TOROUTILITIES_API UToroSaveObject : public UObject
{
	GENERATED_BODY()

public:

	UToroSaveObject(): SaveStatus(ESaveGameStatus::None) {}

	static UToroSaveObject* Create(UObject* Owner, const TSubclassOf<UToroSaveObject>& Class, const FGameplayTag& Tag);

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSaveActivity, const UToroSaveObject*, const ESaveGameActivity);
	FOnSaveActivity OnSaveActivity;

	FString GetSaveName() const;
	FGameplayTag GetSaveTag() const { return SaveTag; }
	ESaveGameStatus GetStatus() const { return SaveStatus; }

	virtual void DeleteData();
	virtual void OnCreation() {}
	virtual void SaveObject(const TFunction<void(const ESaveGameStatus)>& Callback);
	virtual void LoadObject(const TFunction<void(const ESaveGameStatus)>& Callback);

protected:

	UPROPERTY() FString SavePath;
	UPROPERTY() FGameplayTag SaveTag;
	UPROPERTY() ESaveGameStatus SaveStatus;

	virtual void SerializeData(FArchive& Ar) {}
};
