// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "3DWidgetActor.generated.h"

UCLASS(MinimalAPI, ConversionRoot, ComponentWrapperClass, HideCategories = (Input), ShowCategories = ("Input|MouseInput", "Input|TouchInput"), DisplayName = "3D Widget Actor", meta = (ChildCanTick))
class A3DWidgetActor : public AActor
{
	GENERATED_BODY()

public:

	A3DWidgetActor()
	{
		PrimaryActorTick.bCanEverTick = false;
		PrimaryActorTick.bStartWithTickEnabled = false;
		WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	}

	UFUNCTION(BlueprintCallable, Category = "3D Widget Actor", meta = (DeterminesOutputType = "Class", DynamicOutputParam = "ReturnValue"))
	UUserWidget* GetWidgetAs(const TSubclassOf<UUserWidget> Class) const
	{
		UUserWidget* Widget = WidgetComponent->GetWidget();
		if (Widget && (!Class || Widget->IsA(Class))) return Widget;
		return nullptr;
	}

	template<typename T = UUserWidget>
	T* GetUserWidget() const { return Cast<T>(WidgetComponent->GetWidget()); }
	
	UWidgetComponent* GetWidgetComponent() { return WidgetComponent; }

private:
	
	UPROPERTY(Category = "3DWidgetActor", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TObjectPtr<UWidgetComponent> WidgetComponent;
};
