// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveTypes.h"
#include "Helpers/ClassGetters.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroSaveManager.generated.h"

UCLASS(BlueprintType, DisplayName = "Save Manager")
class TOROUTILITIES_API UToroSaveManager final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UToroSaveManager(): ActiveGameSave(SaveTags::TAG_Game) {}

	GAME_INSTANCE_SUBSYSTEM_GETTER(UToroSaveManager)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveSystem)
		FGameplayTag ActiveGameSave;

	UFUNCTION(BlueprintPure, Category = SaveSystem)
		UToroSaveObject* FindSaveObject(UPARAM(meta = (Categories = "Save")) const FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = SaveSystem)
		bool CreateSaveObject(const TSubclassOf<UToroSaveObject> Class, const FGameplayTag InTag);

	template <typename T = UToroSaveObject>
	T* FindSaveObject(const FGameplayTag& InTag) { return Cast<T>(FindSaveObject(InTag)); }

	template <typename T>
	static T* GetSaveObject(const UObject* ContextObject, const FGameplayTag& Tag)
	{
		if (UToroSaveManager* Manager = UToroSaveManager::Get(ContextObject))
		{
			return Manager->FindSaveObject<T>(Tag);
		}
		return nullptr;
	}

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSaveIO, const UToroSaveObject*, const ESaveGameActivity);
	FOnSaveIO OnSaveIO;

private:

	UPROPERTY(Transient) TMap<FGameplayTag, TObjectPtr<UToroSaveObject>> SaveObjects;

	void OnSaveActivity(const UToroSaveObject* Save, const ESaveGameActivity Type) const;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
