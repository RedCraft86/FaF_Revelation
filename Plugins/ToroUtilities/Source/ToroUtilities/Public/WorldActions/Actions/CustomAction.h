// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "WorldActions/WorldActionBase.h"
#include "CustomAction.generated.h"

UCLASS(Abstract, EditInlineNew, Blueprintable, BlueprintType)
class TOROUTILITIES_API UWorldActionObject final : public UObject
{
	GENERATED_BODY()

	friend struct FWACustomObject;

public:

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		bool bCanEverTick;

	UFUNCTION(BlueprintImplementableEvent)
		void RunAction();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnBeginPlay();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnTick(const float DeltaTime);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (ForceAsFunction = true))
		void OnConstruction();
	
	UFUNCTION(BlueprintImplementableEvent, meta = (ForceAsFunction = true))
		void OnPostEditChange();

	virtual UWorld* GetWorld() const override
	{
		UWorld* World = Super::GetWorld();
		if (!World) World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
		if (!World) World = GEngine->GetCurrentPlayWorld();
		return World;
	}

protected:
	UPROPERTY(Transient) TObjectPtr<UObject> WorldContext;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		OnPostEditChange();
	}
#endif
};

USTRUCT(BlueprintType, DisplayName = "Custom")
struct TOROUTILITIES_API FWACustomObject final : public FWorldActionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Instanced, Category = CustomAction)
		TObjectPtr<UWorldActionObject> ActionObject;

	FWACustomObject() {}
	virtual bool ShouldTick() const override;

protected:

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
	virtual void OnTick(const float DeltaTime) override;
	virtual void OnConstruction() override;

	UObject* GetMutableContext() const { return const_cast<UObject*>(GetWorldContext()); }
};