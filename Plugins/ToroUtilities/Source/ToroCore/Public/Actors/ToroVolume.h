// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroActor.h"
#include "GameFramework/Volume.h"
#include "DataTypes/SimpleDelegates.h"
#include "Components/BrushComponent.h"
#include "ToroVolume.generated.h"

UCLASS()
class TOROCORE_API AToroVolume : public AVolume, public IActorStateInterface, public IGuidInterface
{
	GENERATED_BODY()

public:

	AToroVolume();

	UPROPERTY(BlueprintAssignable, DisplayName = "On Enable State Changed")
		FBoolStateDelegateBP OnEnableStateChangedBP;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Enable State Changed")
		void EnableStateChangedBP(const bool bState);

	FBoolStateDelegate OnEnableStateChanged;

	virtual void SetEnabledState_Implementation(const bool bInEnabled) override;
	virtual bool GetEnabledState_Implementation() const override { return bEnabled; }
	virtual FGuid GetUniqueGUID_Implementation() override { return UniqueGuid; }

protected:
#if WITH_EDITOR
	DECLARE_VISUAL_COMPONENT
#endif
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = Tick)
		bool bTickInEditor = false;
#endif

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bEnabled;

	UPROPERTY(EditAnywhere, Category = Actor, NonPIEDuplicateTransient, TextExportTransient, NonTransactional)
		FGuid UniqueGuid;

	void BroadcastStateChanged();
	virtual void EnableStateChanged(const bool bState);

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual bool ShouldTickIfViewportsOnly() const override { return bTickInEditor; }
#endif
};
