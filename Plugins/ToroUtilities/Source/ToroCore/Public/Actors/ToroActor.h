// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "ToroActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorEnableChangedSignature, const bool, bEnabled);

#define ON_ENABLE_STATE_CHANGED { OnEnableStateChanged(bEnabled); OnEnableStateChangedEvent.Broadcast(bEnabled); EnableStateChangedEvent(bEnabled); OnEnableStateChangedBP.Broadcast(bEnabled); }

UCLASS(Abstract)
class TOROCORE_API AToroActor : public AActor
{
	GENERATED_BODY()

#if WITH_EDITOR
	friend class FToroActorCustomization;
#endif
public:

	AToroActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<USceneComponent> SceneRoot;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = "Subobjects")
		TObjectPtr<UBillboardComponent> DefaultIconBillboard;
#endif

	UPROPERTY(BlueprintAssignable, DisplayName = "On Enable State Changed")
		FActorEnableChangedSignature OnEnableStateChangedBP;
	
	UFUNCTION(BlueprintCallable, Category = "Actor")
		virtual void SetEnabled(const bool bInEnabled);
	
	UFUNCTION(BlueprintPure, Category = "Actor")
		bool IsEnabled() const { return bEnabled; }
	
	UFUNCTION(BlueprintPure, Category = "Actor")
		FGuid GetRuntimeGuid() const { return RuntimeGuid; }

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Enable State Changed")
		void EnableStateChangedEvent(const bool bState);
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FActorEnableChanged, const bool);
	FActorEnableChanged OnEnableStateChangedEvent;

protected:

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayPriority = -10))
		bool bEnabled;
	
	UPROPERTY(EditAnywhere, Category = "Settings", NonPIEDuplicateTransient, TextExportTransient, NonTransactional, AdvancedDisplay, meta = (DisplayPriority = 10))
		FGuid RuntimeGuid;

	UPROPERTY(EditAnywhere, Category = "Collision")
		bool bStartWithCollisionEnabled;

#if WITH_EDITORONLY_DATA
	UPROPERTY() FString BillboardIcon = TEXT("/Engine/EditorResources/EmptyActor.EmptyActor");
	UPROPERTY(Transient) TSoftObjectPtr<UTexture2D> BillboardIconTexture;
#endif
	
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void OnEnableStateChanged(const bool bIsEnabled);
};