// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GTActor.h"
#include "CCTVCamera.h"
#include "Data/MathTypes.h"
#include "InputActionValue.h"
#include "Interaction/InteractionInterface.h"
#include "CCTVMonitor.generated.h"

class UInputAction;

UCLASS(Abstract)
class FAF_REV_API ACCTVMonitor final : public AGTActor, public IInteractionInterface
{
	GENERATED_BODY()

	friend class ACCTVCamera;

public:

	ACCTVMonitor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<UAudioComponent> MonitorAudio;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<UStaticMeshComponent> MonitorMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<class UWidgetComponent> MonitorWidget;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<class UCameraComponent> MonitorCamera;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (GetOptions = "GetCameraOptions"))
		FName DefaultCamera;

	UPROPERTY(EditAnywhere, Category = "Settings")
		TMap<FName, TObjectPtr<ACCTVCamera>> Cameras;

	UPROPERTY(EditAnywhere, Category = "Settings", AdvancedDisplay)
		TObjectPtr<UTextureRenderTarget2D> CaptureRT;

	UPROPERTY(EditAnywhere, Category = "Settings", AdvancedDisplay, meta = (DisplayThumbnail = false))
		TObjectPtr<UInputAction> TurnInput;
	
	UPROPERTY(EditAnywhere, Category = "Settings", AdvancedDisplay, meta = (DisplayThumbnail = false))
		TObjectPtr<UInputAction> ExitInput;

	UFUNCTION(BlueprintPure, Category = "CCTV")
		TArray<FName> GetCameraOptions() const;

	UFUNCTION(BlueprintCallable, Category = "CCTV")
		void ChangeCamera(const FName InKey);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "CCTV")
		void PlayMonitorAudio(USoundBase* Sound, const float Volume) const;

	UFUNCTION(BlueprintPure, Category = "CCTV")
		bool IsOnActiveCamera() const { return ActiveCamera.Value && ActiveCamera.Value->IsFunctional(); }

	UFUNCTION(BlueprintPure, Category = "CCTV")
		FName GetActiveCamera() const { return ActiveCamera.Value ? ActiveCamera.Key : NAME_None; }

	UFUNCTION(BlueprintPure, Category = "CCTV")
		bool IsSeeingEnemies() const { return IsOnActiveCamera() ? ActiveCamera.Value->IsSeeingEnemies() : false; }

	UFUNCTION(BlueprintPure, Category = "CCTV")
		FText GetCameraName() const { return IsOnActiveCamera() ? ActiveCamera.Value->DisplayName : FText::GetEmpty(); }

	UFUNCTION(BlueprintImplementableEvent)
		void ZoomIntoMonitor(APlayerController* Controller);

	UFUNCTION(BlueprintImplementableEvent)
		void ZoomOutOfMonitor(APlayerController* Controller, const AActor* Target);
	
protected:

	bool bZoomedIn;
	FTimerHandle ChangeCameraHandle;
	FGTInterpScalar ChangeCameraStatic;
	TObjectPtr<AFRPlayerBase> PlayerChar;
	TPair<FName, TObjectPtr<ACCTVCamera>> ActiveCamera;

	virtual bool GetInteractionInfo_Implementation(FInteractionInfo& Info) override;
	virtual void OnBeginInteract_Implementation(AFRPlayerBase* Player, const FHitResult& HitResult) override;

	void InputBinding_Turn(const FInputActionValue& InValue);
	void InputBinding_Exit(const FInputActionValue& InValue);
	
	void UpdateCameraStatic();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnEnableStateChanged(const bool bIsEnabled) override;
};
