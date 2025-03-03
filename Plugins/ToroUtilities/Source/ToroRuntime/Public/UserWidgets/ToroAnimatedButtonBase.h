// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ToroAnimatedButtonBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleSubWidgetDelegateBP);

UCLASS(Abstract)
class TORORUNTIME_API UToroAnimatedButtonBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UToroAnimatedButtonBase(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
	{
#if WITH_EDITORONLY_DATA
		PaletteCategory = NSLOCTEXT("UMG", "Common", "Common");
#endif
	}

	UPROPERTY(BlueprintAssignable, DisplayName = "On Clicked")
		FSimpleSubWidgetDelegateBP OnClickedBP;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UButton> Button;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ButtonAnim;

	DECLARE_MULTICAST_DELEGATE(FOnClicked);
	FOnClicked OnClicked;

	virtual void NativeOnInitialized() override;
	
protected:

	UFUNCTION() void OnButtonClicked();
	UFUNCTION() void OnButtonHovered();
	UFUNCTION() void OnButtonUnhovered();
};
