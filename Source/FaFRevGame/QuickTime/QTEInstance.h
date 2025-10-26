// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "QTEWidget.h"
#include "UObject/Object.h"
#include "Framework/ToroPlayerController.h"
#include "QTEInstance.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UQTEInstance : public UObject
{
	GENERATED_BODY()

public:

	UQTEInstance(): bFinished(false) {}

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UQTEWidget> WidgetClass;

	UFUNCTION(BlueprintPure, Category = QTE)
		virtual float GetProgress() { return 0.0f; }

	UFUNCTION(BlueprintPure, Category = QTE)
		virtual bool IsFinished() { return bFinished; }

	UFUNCTION(BlueprintNativeEvent, Category = QTE)
		void BeginQTE(UWorld* World);
	virtual void BeginQTE_Implementation(UWorld* World)
	{
		Controller = AToroPlayerController::Get(World);
	}

	UFUNCTION(BlueprintNativeEvent, Category = QTE)
		void TickQTE(const float DeltaTime);
	virtual void TickQTE_Implementation(const float DeltaTime) {}

	UFUNCTION(BlueprintNativeEvent, Category = QTE)
		void KeyPressed(const FKey& Key);
	virtual void KeyPressed_Implementation(const FKey& Key) {}

	UFUNCTION(BlueprintCallable, Category = QTE)
	virtual void MarkFinished(const bool bSuccess)
	{
		bFinished = true;
		if (OnFinished.IsBound())
		{
			OnFinished.Execute(bSuccess);
			OnFinished.Unbind();
		}
	}

	virtual void UnbindDelegates() {}

	DECLARE_DELEGATE_OneParam(FOnFinishQTE, const bool)
	FOnFinishQTE OnFinished;

protected:

	bool bFinished;
	TObjectPtr<AToroPlayerController> Controller;
};
