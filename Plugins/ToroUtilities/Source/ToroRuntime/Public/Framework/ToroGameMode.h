// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "ToroGameInstance.h"
#include "GameplayTagContainer.h"
#include "ToroNarrativeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Inventory/InventoryComponent.h"
#include "ToroGameMode.generated.h"

UCLASS()
class TORORUNTIME_API AToroGameMode : public AGameModeBase
{
	GENERATED_BODY()

	friend class ALevelZoneVolume;

public:

	AToroGameMode();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UInventoryComponent> Inventory;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UToroNarrativeComponent> Narrative;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Game Mode"))
		static EToroValidPins GetToroGameMode(AToroGameMode*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroGameMode>& Class);

	template <typename T = AToroGameMode>
	static T* Get(const UObject* WorldContextObject)
	{
		return Cast<T>(UGameplayStatics::GetGameMode(WorldContextObject));
	}

	/* Returns the last cached zone a character is in. */
	UFUNCTION(BlueprintCallable, Category = Game)
		FGameplayTag GetZoneFromCharacter(UPARAM(meta = (Categories = "Character")) const FGameplayTag InCharacter) const;

	/* Returns the last cached characters in a zone. */
	UFUNCTION(BlueprintCallable, Category = Game)
		TSet<FGameplayTag> GetCharactersInZone(UPARAM(meta = (Categories = "Zone")) const FGameplayTag InZone) const;
	
protected:

	UPROPERTY(Transient) TObjectPtr<UToroGameInstance> GameInstance;
	UPROPERTY(Transient) TMap<FGameplayTag, FGameplayTag> CharacterToZone;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
