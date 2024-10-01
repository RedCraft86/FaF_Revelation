// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Data/InlineCurves.h"
#include "SMStateMachineComponent.h"
#include "GameFramework/Character.h"
#include "FRCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCharacterAudioEvent, const class AFRCharacter*, Character, const UAudioComponent*, Component, const FName, Key);

UCLASS(Abstract, DisplayName = "Character Base")
class FAF_REV_API AFRCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AFRCharacter();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<UAudioComponent> FootstepAudio;

	UPROPERTY(EditAnywhere, Category = "Settings")
		FInlineFloatCurve AudioVolumeCurve;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Settings", AdvancedDisplay)
		bool bDebugAudio = false;
	
	UPROPERTY(EditAnywhere, Category = "Settings", AdvancedDisplay)
		FColor DebugColor = FColor::MakeRandomColor();
#endif

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", AdvancedDisplay)
		TSoftObjectPtr<class AFRPlayerBase> PlayerChar;

	UPROPERTY(BlueprintAssignable)
		FOnCharacterAudioEvent OnAudioPlayed;

	UFUNCTION(BlueprintCallable, Category = "CharacterAI")
		void PlaySmartAudio(const FName AudioKey);

	UFUNCTION(BlueprintCallable, Category = "CharacterAI")
		void DisableAI();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
		USMStateMachineComponent* GetLogicComponent() const;

	UFUNCTION(BlueprintPure, BlueprintImplementableEvent, Category = "AI")
		TMap<FName, float> GetSpeedPoints() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "AI")
		TMap<FName, UAudioComponent*> GetKeyedAudio() const;
	TMap<FName, UAudioComponent*> GetKeyedAudio_Implementation() const;
	
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "AI")
		USceneComponent* GetLookAtComponent() const;
	USceneComponent* GetLookAtComponent_Implementation() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "AI")
		USceneComponent* GetPlayerLookTarget() const;
	USceneComponent* GetPlayerLookTarget_Implementation() const { return nullptr; }

protected:

	virtual void OnConstruction(const FTransform& Transform) override;
};
