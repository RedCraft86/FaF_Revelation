// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "WidgetActor3D.generated.h"

UCLASS(ConversionRoot, ComponentWrapperClass, HideCategories = (Input, Physics, HLOD), DisplayName = "3D Widget Actor", meta = (ChildCanTick))
class TOROCORE_API AWidgetActor3D : public AActor
{
	GENERATED_BODY()

public:

	AWidgetActor3D()
	{
		PrimaryActorTick.bCanEverTick = false;
		PrimaryActorTick.bStartWithTickEnabled = false;
		
		WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
		RootComponent = WidgetComponent;

		SetCanBeDamaged(false);
	}

	UFUNCTION(BlueprintCallable, Category = "3D Widget Actor", meta = (DeterminesOutputType = "Class", DynamicOutputParam = "ReturnValue"))
	UUserWidget* GetWidget(const TSubclassOf<UUserWidget> Class) const
	{
		UUserWidget* Widget = WidgetComponent->GetWidget();
		if (Widget && (!Class || Widget->IsA(Class))) return Widget;
		return nullptr;
	}

	template<typename T = UUserWidget>
	T* GetUserWidget() const { return Cast<T>(WidgetComponent->GetWidget()); }
	
	UWidgetComponent* GetWidgetComponent() { return WidgetComponent; }

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "3D Widget Actor", meta = (AllowPrivateAccess = true))
		TObjectPtr<UWidgetComponent> WidgetComponent;
};