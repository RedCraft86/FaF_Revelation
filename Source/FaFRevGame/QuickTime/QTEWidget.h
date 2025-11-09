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

	UQTEWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
	{
		bAutoPush = false;
		ContainerClass = UQTEWidgetContainer::StaticClass();
		UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	UFUNCTION(BlueprintImplementableEvent)
		void OnInitWidget(UQTEManager* QTE_Manager);

	UFUNCTION(BlueprintImplementableEvent)
		void OnDeinitWidget();

	void ShowWidget(UQTEManager* InManager)
	{
		if (InManager || InManager != Manager)
		{
			Manager = InManager;
		}

		PushWidget();
	}

	virtual void PopWidget() override
	{
		Super::PopWidget();
		OnDeinitWidget();
	}

protected:

	UPROPERTY(BlueprintReadOnly, Category = QTE)
		TObjectPtr<UQTEManager> Manager;

	virtual void PushWidget() override
	{
		Super::PushWidget();
		OnInitWidget(Manager);
	}
};
