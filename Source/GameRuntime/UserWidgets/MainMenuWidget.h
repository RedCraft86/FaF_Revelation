// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "MainMenuWidget.generated.h"

class UCommonButtonBase;

UCLASS(Abstract, Blueprintable, BlueprintType)
class UMainMenuWidget final : public UToroActivatableWidget
{
	GENERATED_BODY()

public:

	UMainMenuWidget(const FObjectInitializer& ObjectInit);

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonButtonBase> PlayButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonButtonBase> SettingsButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonButtonBase> ExtrasButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonButtonBase> QuitButton;

	void OnPlayClicked();
	void OnSettingsClicked() const;
	void OnExtrasClicked();
	void OnQuitClicked();

	UE5Coro::TCoroutine<> FadeOut(const TFunction<void()>& Callback);

	virtual void NativeConstruct() override;
};
