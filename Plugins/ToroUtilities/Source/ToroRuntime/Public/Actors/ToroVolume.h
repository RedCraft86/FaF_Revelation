// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroActor.h"
#include "GameFramework/Volume.h"
#include "ToroVolume.generated.h"

UCLASS()
class TORORUNTIME_API AToroVolume : public AVolume
{
	GENERATED_BODY()

public:

	AToroVolume();

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = "Subobjects")
		TObjectPtr<UBillboardComponent> DefaultIconBillboard;
#endif

	UPROPERTY(BlueprintAssignable, DisplayName = "On Enable State Changed")
		FActorEnableChangedSignature OnEnableStateChangedBP;
	
	UFUNCTION(BlueprintCallable, Category = "Actor")
		void SetEnabled(const bool bInEnabled);
	
	UFUNCTION(BlueprintPure, Category = "Actor")
		bool IsEnabled() const { return bEnabled; }
	
	UFUNCTION(BlueprintPure, Category = "Actor")
		FGuid GetRuntimeGuid() const { return RuntimeGuid; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FActorEnableChanged, const bool);
		FActorEnableChanged OnEnableStateChangedEvent;

protected:

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayPriority = -10))
		bool bEnabled;
	
	UPROPERTY(EditAnywhere, Category = "Settings", NonPIEDuplicateTransient, TextExportTransient, NonTransactional, meta = (DisplayPriority = -10))
		FGuid RuntimeGuid;
	
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void OnEnableStateChanged(const bool bIsEnabled);
};