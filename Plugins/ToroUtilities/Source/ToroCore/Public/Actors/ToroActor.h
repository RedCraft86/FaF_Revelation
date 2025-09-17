// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "ToroActor.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FActorEnableChanged, const bool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorEnableChangedBP, const bool, bEnabled);

UCLASS(Abstract)
class TOROCORE_API AToroActor : public AActor
{
	GENERATED_BODY()

public:

	AToroActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(BlueprintAssignable, DisplayName = "On Enable State Changed")
		FActorEnableChangedBP OnEnableStateChangedBP;

	UFUNCTION(BlueprintCallable, Category = Actor)
		virtual void SetEnabled(const bool bInEnabled);

	UFUNCTION(BlueprintPure, Category = Actor)
		bool IsEnabled() const { return bEnabled; }

	UFUNCTION(BlueprintPure, Category = Actor)
		FGuid GetRuntimeGuid() const { return RuntimeGuid; }

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Enable State Changed")
		void EnableStateChangedBP(const bool bState);

	FActorEnableChanged OnEnableStateChanged;

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = Tick)
		bool bTickInEditor = false;
#endif

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bEnabled;

	UPROPERTY(EditAnywhere, Category = Actor, NonPIEDuplicateTransient, TextExportTransient, NonTransactional)
		FGuid RuntimeGuid;

	UPROPERTY(EditAnywhere, Category = Actor)
		bool bStartWithCollision;

	void BroadcastStateChanged();
	virtual void EnableStateChanged(const bool bIsEnabled);

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITORONLY_DATA
	virtual bool ShouldTickIfViewportsOnly() const override { return bTickInEditor; }
#endif
};
