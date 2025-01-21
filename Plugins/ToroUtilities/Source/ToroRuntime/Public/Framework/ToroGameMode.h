// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "ToroGameInstance.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
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

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Game Mode"))
		static EToroValidPins GetToroGameMode(AToroGameMode*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroGameMode>& Class);

	template <typename T = AToroGameMode>
	static T* Get(const UObject* WorldContextObject)
	{
		return Cast<T>(UGameplayStatics::GetGameMode(WorldContextObject));
	}

	/* Returns the last cached room a character is in. */
	UFUNCTION(BlueprintCallable, Category = Game)
		FGameplayTag GetRoomFromCharacter(UPARAM(meta = (Categories = "Character")) const FGameplayTag& InCharacter) const;

protected:

	UPROPERTY(Transient) TObjectPtr<UToroGameInstance> GameInstance;
	UPROPERTY(Transient) TMap<FGameplayTag, FGameplayTag> CharacterZoneMap;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
