﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h"
#include "ToroUserWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FToroWidgetAddRemoveSignature);

UCLASS(Abstract)
class TORORUNTIME_API UToroUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UToroUserWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnimOptional))
		TObjectPtr<UWidgetAnimation> FadeAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnimOptional))
		TObjectPtr<UWidgetAnimation> AutoHideAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UserWidget)
		int32 ZOrder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UserWidget)
		bool bAutoAdd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UserWidget, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float AutoHideInterval;
	
	UFUNCTION(BlueprintCallable, Category = UserWidget, meta = (WorldContext = "ContextObject"))
		static UToroUserWidget* CreateSmartWidget(APlayerController* Owner, const TSubclassOf<UToroUserWidget> Class);

	template <typename T = UToroUserWidget>
	static T* CreateNew(APlayerController* Owner)
	{
		return Cast<T>(CreateSmartWidget(Owner, T::StaticClass()));
	}

	UFUNCTION(BlueprintPure, Category = UserWidget)
		bool IsHidden() const { return bHidden; }
	
	UFUNCTION(BlueprintCallable, Category = UserWidget)
		void SetHidden(const bool bInHidden);
	
	UFUNCTION(BlueprintCallable, Category = UserWidget, meta = (Keywords = "add widget"))
		void AddUserWidget(FToroWidgetAddRemoveSignature OnFinished);

	UFUNCTION(BlueprintCallable, Category = UserWidget, meta = (Keywords = "remove widget"))
		void RemoveUserWidget(FToroWidgetAddRemoveSignature OnFinished);
	
	UFUNCTION(BlueprintImplementableEvent)
		bool ShouldAutoHide() const;
	
	template<typename T = AGameModeBase>
	T* GetGameMode() { return Cast<T>(GameMode); }
	
	template<typename T = APlayerController>
	T* GetPlayerController() { return Cast<T>(PlayerController); }

	virtual void InitWidget();
	virtual void AddWidget(const TFunction<void()>& OnFinished = nullptr);
	virtual void RemoveWidget(const TFunction<void()>& OnFinished = nullptr);
	
protected:
	
	UPROPERTY(BlueprintReadOnly, Category = UserWidget, meta = (AllowPrivateAccess = true))
		TObjectPtr<AGameModeBase> GameMode;
	
	UPROPERTY(BlueprintReadOnly, Category = UserWidget, meta = (AllowPrivateAccess = true))
		TObjectPtr<APlayerController> PlayerController;

	UPROPERTY() bool bRemoving;
	UPROPERTY() bool bHidden;
	UPROPERTY() bool bCachedHidden;
	UPROPERTY() float HideCheck;

	virtual void HandleAutoHiding(const bool bInHidden);
	virtual bool ShouldHide() const { return bHidden || ShouldAutoHide(); }
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
