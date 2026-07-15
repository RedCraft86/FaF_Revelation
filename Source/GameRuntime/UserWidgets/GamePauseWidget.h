// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "CommonButtonBase.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "GamePauseWidget.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class UGamePauseWidget final : public UToroActivatableWidget
{
	GENERATED_BODY()

public:

	UGamePauseWidget(const FObjectInitializer& ObjectInit);

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<class UTextBlock> MenuTitle;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<class UWidgetSwitcher> MenuSwitcher;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonButtonBase> ContextButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonButtonBase> ObjectiveButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonButtonBase> ArchivesButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonButtonBase> InventoryButton;

	void OnContextClicked() const;
	void OnObjectiveClicked() const;
	void OnArchivesClicked() const;
	void OnInventoryClicked() const;

	virtual void NativeConstruct() override;
};
