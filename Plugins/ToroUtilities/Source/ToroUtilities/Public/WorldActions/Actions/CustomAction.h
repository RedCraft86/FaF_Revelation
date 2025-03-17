// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "WorldActions/WorldActionBase.h"
#include "CustomAction.generated.h"

UCLASS(Abstract, EditInlineNew, Blueprintable, BlueprintType)
class TOROUTILITIES_API UWorldActionObject final : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		bool bCanEverTick;

	UFUNCTION(BlueprintImplementableEvent)
		void RunEvent();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnBeginPlay();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnTick(const float DeltaTime);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (ForceAsFunction = true))
		void OnPostEditChange();

	UPROPERTY(Transient) TObjectPtr<UObject> ContextObject;
	virtual UWorld* GetWorld() const override
	{
#if WITH_EDITOR
		if (!FApp::IsGame()) return GEngine->GetCurrentPlayWorld();
#endif
		UWorld* World = Super::GetWorld();
		if (!World && ContextObject) World = ContextObject->GetWorld();
		if (!World) World = GEngine ? GEngine->GetCurrentPlayWorld() : GWorld;
		return World;
	}
#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		OnPostEditChange();
	}
#endif
};

USTRUCT(BlueprintType, DisplayName = "Custom")
struct TOROUTILITIES_API FWControlObject final : public FWorldActionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Instanced, Category = CustomAction)
		TObjectPtr<UWorldActionObject> ActionObject;

	FWControlObject() : ActionObject(nullptr) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
	virtual void OnBeginPlay(const UObject* WorldContext) override;
	virtual void OnTick(const UObject* WorldContext, const float DeltaTime) override;
	virtual void OnPostEditChange(const UObject* WorldContext) override;
};
