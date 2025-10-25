// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "QTEWidgetContainer.h"
#include "UserInterface/ToroManagedWidget.h"
#include "QTEWidget.generated.h"

class UQTEManager;

UCLASS(Abstract)
class FAFREVGAME_API UQTEWidget : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UQTEWidget(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		bAutoPush = false;
		ContainerClass = UQTEWidgetContainer::StaticClass();
	}

	void ShowWidget(UQTEManager* InManager)
	{
		Manager = InManager;
		PushWidget();
	}

protected:

	UPROPERTY(BlueprintReadOnly, Category = QTE)
		TObjectPtr<UQTEManager> Manager;
};
