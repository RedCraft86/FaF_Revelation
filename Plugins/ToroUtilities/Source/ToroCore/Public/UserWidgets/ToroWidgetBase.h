﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"
#include "Animation/WidgetAnimation.h"
#include "ToroWidgetBase.generated.h"

#define InitAnim(Anim){ \
		PlayAnimation(Anim); \
		StopAnimation(Anim); \
	}

#define GetAnimDuration(Anim) (Anim->GetEndTime() - Anim->GetStartTime())

UCLASS(Abstract)
class TOROCORE_API UToroWidgetBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	UToroWidgetBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ActivateAnim;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMax = 50, UIMax = 50))
		uint8 ZOrder;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.05f, UIMin = 0.05f))
		float HideCheckInterval;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (DynamicOutputParam = "ReturnValue", DeterminesOutput = "Class", AutoCreateRefTerm = "Class"))
		static UToroWidgetBase* CreateToroWidget(APlayerController* Controller, const TSubclassOf<UToroWidgetBase>& Class);

	UFUNCTION(BlueprintCallable, Category = UserWidget)
		void SetHidden(const bool bInHidden);

	UFUNCTION(BlueprintPure, Category = UserWidget)
		bool WantsHidden() const { return bWantHidden; }

	template <typename T = AGameModeBase>
		T* GetGameMode() const { return Cast<T>(GameMode); }

	template <typename T = APlayerController>
		T* GetPlayerController() const { return Cast<T>(GetOwningPlayer()); }

protected:

	UPROPERTY(Transient) TObjectPtr<AGameModeBase> GameMode;
	UPROPERTY() bool bHidden;
	UPROPERTY() bool bWantHidden;
	UPROPERTY() bool bBeforeHidden;
	UPROPERTY() float AutoHideTimer;

	virtual void InitWidget() {}
	virtual bool ShouldBeHidden();
	virtual void UpdateHiddenState();
	virtual bool CanCreateWidget() const { return true; }
	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
};