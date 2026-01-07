// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/MathTypes.h"
#include "Inventory/InventoryAsset.h"
#include "Interaction/InteractableActor.h"
#include "Components/CurvePlayer/CurvePlayerFloat.h"
#include "Components/BoxComponent.h"
#include "Navigation/NavLinkProxy.h"
#include "DoorBase.generated.h"

UCLASS(NotBlueprintable)
class FAFREVGAME_API ADoorLink final : public ANavLinkProxy
{
	GENERATED_BODY()

public:
	
	FSmartLinkReachedSignature& GetOnSmartLinkReached() { return OnSmartLinkReached; }
};

UCLASS(Abstract, meta = (ChildCanTick = true))
class FAFREVGAME_API ADoorBase : public AInteractableActor
{
	GENERATED_BODY()

public:

	ADoorBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UBoxComponent> DoorRange;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> OpenAudio;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> CloseAudio;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UChildActorComponent> DoorLinkComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UCurvePlayerFloat> CurvePlayer;

	UFUNCTION(BlueprintCallable, Category = Door)
		void SetOpened(const bool bInOpened, const bool bImmediate = false);

	UFUNCTION(BlueprintPure, Category = Door)
		bool IsOpened() const { return bOpened; }

	UFUNCTION(BlueprintPure, Category = Door)
		bool IsLocked() const { return KeyAsset && !bOpened; }

	UFUNCTION(BlueprintPure, Category = Door)
		AActor* GetLastInteractor() const { return Interactor.Get(); }

	UFUNCTION(BlueprintNativeEvent)
		void OpenStateChanged(const bool bState, const bool bImmediate);
	void OpenStateChanged_Implementation(const bool bState, const bool bImmediate);

protected:
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayPriority = -1))
		bool bStartOpened;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayPriority = -1))
		TObjectPtr<UInventoryAsset> KeyAsset;

	UPROPERTY(EditAnywhere, Category = "Settings|Animation", meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float PlayRate;

	UPROPERTY(EditAnywhere, Category = "Settings|Animation", meta = (XAxisName = "Time", YAxisName = "Alpha"))
		FInlineFloatCurve Animation;

	UPROPERTY(EditAnywhere, Category = "Settings", AdvancedDisplay)
		FText LockedLabel;

	UPROPERTY(EditAnywhere, Category = "Settings", AdvancedDisplay)
		bool bSmartLink;

	UPROPERTY(EditAnywhere, Category = "Settings", AdvancedDisplay)
		FNavigationLink NavPoints;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Tools)
		bool bFlipXY = true;

	UPROPERTY(EditAnywhere, Category = Tools)
		float NavPointOffsetMulti = 0.5f;

	UFUNCTION(CallInEditor, Category = Tools)
		void TryDetermineNavLinks();
#endif

	bool bOpened;
	FTimerHandle DoorLinkTimer;
	TObjectPtr<ADoorLink> DoorLink;
	TWeakObjectPtr<AActor> Interactor;
	TObjectPtr<UInventoryManager> Inventory;
	
	UFUNCTION() 
	void OnEntityReachedDoor(AActor* Entity, const FVector& Dest);

	virtual bool GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info) override;
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit) override;
	virtual void OnPawnInteract_Implementation(AActor* Pawn) override;

	virtual void BeginPlay() override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
