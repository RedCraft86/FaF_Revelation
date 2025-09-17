// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Interfaces/GuidInterface.h"
#if WITH_EDITOR
#include "Components/EditorVisual.h"
#endif
#include "ToroActor.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FActorEnableChanged, const bool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorEnableChangedBP, const bool, bState);

UCLASS(Abstract)
class TOROCORE_API AToroActor : public AActor, public IGuidInterface
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

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Enable State Changed")
		void EnableStateChangedBP(const bool bState);

	FActorEnableChanged OnEnableStateChanged;

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

	UPROPERTY(EditAnywhere, Category = Actor)
		bool bStartWithCollision;

	void BroadcastStateChanged();
	virtual void EnableStateChanged(const bool bState);

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITORONLY_DATA
	virtual bool ShouldTickIfViewportsOnly() const override { return bTickInEditor; }
#endif
};
