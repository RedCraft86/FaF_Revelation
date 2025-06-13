// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroActor.h"
#include "GameFramework/Volume.h"
#include "ToroVolume.generated.h"

UCLASS(Abstract, meta = (ChildCanTick))
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
		void EnableStateChangedBP(const bool bState);

	FActorEnableChangedNative OnEnableStateChanged;

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UEditorVisualComponent> EdVisualComp;

	UPROPERTY(EditDefaultsOnly, Category = Tick)
		bool bTickInEditor = false;
#endif

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bEnabled;

	UPROPERTY(EditAnywhere, Category = Actor, NonPIEDuplicateTransient, TextExportTransient, NonTransactional)
		FGuid RuntimeGuid;

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void EnableStateChanged(const bool bIsEnabled);
#if WITH_EDITORONLY_DATA
	virtual bool ShouldTickIfViewportsOnly() const override { return bTickInEditor; }
#endif
};
