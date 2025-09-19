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

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnimOptional))
		TObjectPtr<UWidgetAnimation> FadeAnim;

	UPROPERTY(Transient)
		TObjectPtr<AGameModeBase> GameMode;

	virtual void InitWidget(APlayerController* Controller);
	virtual bool CanCreateWidget(const UObject* ContextObject) const { return IsValid(ContextObject); }

	virtual void NativeConstruct() override;
	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
};
