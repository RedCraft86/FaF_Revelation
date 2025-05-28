// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveObjects.h"
#include "Helpers/ClassGetters.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroSaveManager.generated.h"

UCLASS(BlueprintType, DisplayName = "Save Manager")
class TOROUTILITIES_API UToroSaveManager final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UToroSaveManager(): ActiveGameSave(Tag_Save_Game) {}
	GAME_INSTANCE_SUBSYSTEM_GETTER(UToroSaveManager);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveIO, const UToroSaveObject*, Save, const ESaveGameActivity, Type);
	UPROPERTY(BlueprintAssignable) FOnSaveIO OnSaveIO;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveSystem)
		FGameplayTag ActiveGameSave;

	UFUNCTION(BlueprintPure, Category = SaveSystem)
		UToroSaveObject* GetSaveObject(UPARAM(meta = (Categories = "Save")) const FGameplayTag InTag);

	template <typename T = UToroSaveObject>
	T* GetSaveObject(const FGameplayTag& InTag) { return Cast<T>(GetSaveObject(InTag)); }

private:

	UPROPERTY(Transient) TMap<FGameplayTag, TObjectPtr<UToroSaveObject>> SaveObjects;

	void OnSaveActivity(const UToroSaveObject* Save, const ESaveGameActivity Type) const;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
