// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Navigation/NavLinkProxy.h"
#include "DoorLink.generated.h"

UCLASS(NotBlueprintable, PrioritizeCategories = (Tools))
class FAFREVGAME_API ADoorLink final : public ANavLinkProxy
{
	GENERATED_BODY()
	
	friend class ADoorBase;

public:

	ADoorLink();

protected:

	UPROPERTY(VisibleAnywhere, Category = Settings)
		TObjectPtr<ADoorBase> LinkedDoor;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Tools)
		bool bSwapXY = true;

	UPROPERTY(EditAnywhere, Category = Tools)
		float OffsetFactor = 0.5f;

	UFUNCTION(CallInEditor, Category = Tools)
		void AutoTarget();
#endif

	UFUNCTION() void OnPawnReached(AActor* Pawn, const FVector& Dest);

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
};
