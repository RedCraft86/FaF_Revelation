// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "QTEWidget.h"
#include "UObject/Object.h"
#include "QTEInstance.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UQTEInstance : public UObject
{
	GENERATED_BODY()

public:

	UQTEInstance() {}

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UQTEWidget> WidgetClass;

	UFUNCTION(BlueprintPure, Category = QTE)
		virtual float GetProgress() { return 0.0f; }

	UFUNCTION(BlueprintNativeEvent, Category = QTE)
		void BeginQTE(UWorld* World);
	virtual void BeginQTE_Implementation(UWorld* World) {}

	UFUNCTION(BlueprintNativeEvent, Category = QTE)
		void TickQTE(const float DeltaTime);
	virtual void TickQTE_Implementation(const float DeltaTime) {}


	UFUNCTION(BlueprintNativeEvent, Category = QTE)
		void OnKeyPress(const FKey& Key);
	virtual void OnKeyPress_Implementation(const FKey& Key) {}

	UFUNCTION(BlueprintCallable, Category = QTE)
	virtual void MarkFinished(const bool bSuccess)
	{
		if (OnFinished.IsBound())
		{
			OnFinished.Execute(bSuccess);
			OnFinished.Unbind();
		}
	}

	DECLARE_DELEGATE_OneParam(FOnFinishQTE, const bool)
	FOnFinishQTE OnFinished;
};
