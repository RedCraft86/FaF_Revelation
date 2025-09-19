// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"
#include "ToroWidgetBase.generated.h"

#define InitAnim(Anim) { \
		PlayAnimation(Anim); \
		StopAnimation(Anim); \
	}

#define GetAnimDuration(Anim) (Anim->GetEndTime() - Anim->GetStartTime())

UCLASS(Abstract)
class TOROFRAMEWORK_API UToroWidgetBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	UToroWidgetBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = UserInterface, DisplayName = "Create ToroWidget", meta = (DeterminesOutputType = "WidgetClass"))
		static UToroWidgetBase* CreateToroWidget(APlayerController* Owner, const TSubclassOf<UToroWidgetBase> WidgetClass);

	UFUNCTION(BlueprintCallable, Category = UserInterface)
		void FadeInWidget();

	UFUNCTION(BlueprintCallable, Category = UserInterface)
		void FadeOutWidget();

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FadeAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Widget)
		TObjectPtr<AGameModeBase> GameMode;

	UPROPERTY(BlueprintReadOnly, Category = Widget, DisplayName = "Wants To Fade In")
		bool bWantsToShow;

	bool bFadeState;
	float FadeCheckTime;

	void UpdateFadeState();

	virtual void InitWidget(APlayerController* Controller);
	virtual bool ShouldShowWidget() const { return true; }
	virtual bool CanCreateWidget(const UObject* ContextObject) const { return IsValid(ContextObject); }

	virtual void NativeConstruct() override;
	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
