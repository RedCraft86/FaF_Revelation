// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Framework/ToroGameMode.h"
#include "CommonActivatableWidget.h"
#include "ToroWidgetBase.generated.h"

class AToroPlayerController;

UCLASS(Abstract)
class TORORUNTIME_API UToroWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	UToroWidget(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ActivateAnim;

	UPROPERTY(EditAnywhere, Category = Settings)
		uint8 ZOrder;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bGameplayOnly;

	UFUNCTION(BlueprintCallable, Category = UserWidget)
		void SetHidden(const bool bInHidden);
	
	template <typename GameModeT = AToroGameMode>
	GameModeT* GetGameMode() const
	{
		static_assert(TIsDerivedFrom<GameModeT, AToroGameMode>::IsDerived, "UToroWidget::GetGameMode can only get AToroGameMode instances.");
		return Cast<GameModeT>(GameMode);
	}
	
protected:
	
	UPROPERTY(Transient) TObjectPtr<AToroGameMode> GameMode;

	UPROPERTY() bool bHidden;
	UPROPERTY() bool bWantHidden;
	UPROPERTY() bool bBeforeHidden;
	UPROPERTY() float AutoHideTimer;

	virtual void InitWidget();
	virtual bool ShouldBeHidden();
	virtual void UpdateHiddenState();
	
	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "UserWidget")
		static UToroWidget* CreateNew(AToroPlayerController* Owner, const TSubclassOf<UToroWidget> Class);
	
	template <typename WidgetT = UToroWidget>
	static WidgetT* CreateNew(AToroPlayerController* Owner, const TSubclassOf<WidgetT> Class)
	{
		static_assert(TIsDerivedFrom<WidgetT, UToroWidget>::IsDerived, "UToroWidget::CreateNew can only be used to create UToroWidget instances.");
		return Cast<WidgetT>(CreateNew(Owner, Class));
	}
};
