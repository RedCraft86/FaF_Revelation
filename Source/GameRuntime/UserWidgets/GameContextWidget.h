// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UE5Coro.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "GameContextWidget.generated.h"

class UCommonButtonBase;
enum class EGameMapType : uint8;

UCLASS(Abstract, Blueprintable, BlueprintType)
class UGameContextWidget final : public UToroUserWidget
{
	GENERATED_BODY()

public:

	UGameContextWidget(const FObjectInitializer& ObjectInit);

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonButtonBase> ResumeButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonButtonBase> SettingsButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonButtonBase> CheckpointButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonButtonBase> MainMenuButton;

	void OnResumeClicked() const;
	void OnSettingsClicked() const;
	void OnCheckpointClicked();
	void OnMainMenuClicked();

	UE5Coro::TCoroutine<> OpenMap(const EGameMapType Map);

	virtual void NativeConstruct() override;
};
