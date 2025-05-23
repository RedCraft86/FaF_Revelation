﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroActor.h"
#include "GameFramework/Volume.h"
#include "ToroVolume.generated.h"

UCLASS(Abstract)
class TOROCORE_API AToroVolume : public AVolume
{
	GENERATED_BODY()

public:

	AToroVolume();

	UPROPERTY(BlueprintAssignable, DisplayName = "On Enable State Changed")
		FActorEnableChangedSignature OnEnableStateChangedBP;
	
	UFUNCTION(BlueprintCallable, Category = Actor)
		virtual void SetEnabled(const bool bInEnabled);
	
	UFUNCTION(BlueprintPure, Category = Actor)
		bool IsEnabled() const { return bEnabled; }
	
	UFUNCTION(BlueprintPure, Category = Actor)
		FGuid GetRuntimeGuid() const { return RuntimeGuid; }

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Enable State Changed")
		void EnableStateChangedEvent(const bool bState);
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FActorEnableChanged, const bool);
	FActorEnableChanged OnEnableStateChangedEvent;

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UActorIconComponent> ActorEdIcon;
#endif

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -10))
		bool bEnabled;
	
	UPROPERTY(EditAnywhere, Category = Actor, NonPIEDuplicateTransient, TextExportTransient, NonTransactional)
		FGuid RuntimeGuid;
	
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void OnEnableStateChanged(const bool bIsEnabled);
};
