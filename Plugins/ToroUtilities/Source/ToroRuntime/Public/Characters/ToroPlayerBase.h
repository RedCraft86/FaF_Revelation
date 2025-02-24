// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "LightingData.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/ToroGameMode.h"
#include "Components/AudioComponent.h"
#include "Framework/ToroGameInstance.h"
#include "Characters/ToroCharacterBase.h"
#include "Components/PointLightComponent.h"
#include "ToroPlayerBase.generated.h"

class AToroPlayerController;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Player);

UCLASS(Abstract)
class TORORUNTIME_API AToroPlayerBase : public AToroCharacterBase
{
	GENERATED_BODY()

public:
	
	AToroPlayerBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UPointLightComponent> PlayerLight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> FootstepAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPointLightProperties LightSettings;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Player Character"))
		static EToroValidPins GetToroPlayerCharacter(AToroPlayerBase*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroPlayerBase>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroPlayerBase>
	static T* Get(const UObject* WorldContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex));
	}

	UFUNCTION(BlueprintPure, Category = Player)
		AToroPlayerController* GetPlayerController() const;
	
protected:

	UPROPERTY(Transient) TObjectPtr<AToroGameMode> GameMode;
	UPROPERTY(Transient) TObjectPtr<UToroGameInstance> GameInstance;
	
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
