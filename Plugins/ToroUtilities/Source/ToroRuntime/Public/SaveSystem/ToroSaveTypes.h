// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "ToroSaveTypes.generated.h"

class UToroSaveManager;

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
class TORORUNTIME_API UToroSaveObject : public UObject
{
	GENERATED_BODY()

public:

	UToroSaveObject();

	static UToroSaveObject* Create(UToroSaveManager* InOwner, const TSubclassOf<UToroSaveObject>& InClass, const uint8 InSlot);

	uint8 GetSaveSlot() const { return Slot; }
	FString GetSaveName() const { return SaveName; }
	ESaveGameStatus GetStatus() const { return Status; }

	virtual void DeleteData();
	virtual void SaveObject(const TFunction<void(const ESaveGameStatus)>& Callback);
	virtual void LoadObject(const TFunction<void(const ESaveGameStatus)>& Callback);
	virtual FString GetSavePath();

protected:

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		FString SaveName;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		bool bSingleSlot;

	uint8 Slot;
	ESaveGameStatus Status;
	UPROPERTY(Transient) TObjectPtr<UToroSaveManager> Manager;

	void UpdateStatus(const ESaveGameStatus InStatus, const TFunction<void(const ESaveGameStatus)>& Func);

	virtual void ValidateFileName();
	virtual void SerializeData(FArchive& Ar) {}

#if WITH_EDITOR
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
