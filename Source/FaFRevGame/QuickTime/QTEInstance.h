// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "QTEInstance.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UQTEInstance : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = QTE)
		virtual float GetProgress() { return 0.0f; }

	UFUNCTION(BlueprintNativeEvent, Category = QTE)
		void BeginQTE();
	virtual void BeginQTE_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = QTE)
		void TickQTE(const float DeltaTime);
	virtual void TickQTE_Implementation(const float DeltaTime) {}

	UFUNCTION(BlueprintCallable, Category = QTE)
	void MarkFinished(const bool bSuccess)
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
