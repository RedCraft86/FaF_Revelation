// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroActor.h"
#include "GameFramework/Volume.h"
#include "Components/BrushComponent.h"
#include "ToroVolume.generated.h"

UCLASS(Abstract)
class TOROCORE_API AToroVolume : public AVolume, public IGuidInterface
{
	GENERATED_BODY()

public:

	AToroVolume();

	UPROPERTY(BlueprintAssignable, DisplayName = "On Enable State Changed")
		FActorEnableChangedBP OnEnableStateChangedBP;

	UFUNCTION(BlueprintCallable, Category = Actor)
		virtual void SetEnabled(const bool bInEnabled);

	UFUNCTION(BlueprintPure, Category = Actor)
		bool IsEnabled() const { return bEnabled; }

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Enable State Changed")
		void EnableStateChangedBP(const bool bState);

	FActorEnableChanged OnEnableStateChanged;

	virtual FGuid GetUniqueGUID_Implementation() override { return UniqueGuid; }

protected:
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
#if WITH_EDITORONLY_DATA
	virtual bool ShouldTickIfViewportsOnly() const override { return bTickInEditor; }
#endif
};
