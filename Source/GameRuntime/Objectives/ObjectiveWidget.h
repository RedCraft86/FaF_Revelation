// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "ObjectiveDatabase.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "ObjectiveWidget.generated.h"

class UTextBlock;
class UExpressiveTextBlock;

UCLASS(Abstract, Blueprintable, BlueprintType)
class UObjectiveEntryWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class UObjectiveWidget;

public:

	UObjectiveEntryWidget(const FObjectInitializer& ObjectInit);

	DECLARE_DELEGATE_OneParam(FOnHovered, const FObjectiveDbEntry&);
	FOnHovered OnHovered;

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UExpressiveTextBlock> SummaryText;

	FObjectiveDbEntry EntryData;

	void InitWidget(const FGameplayTag& Key);

	virtual void NativeDestruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class UObjectiveWidget final : public UToroUserWidget
{
	GENERATED_BODY()

public:

	UObjectiveWidget(const FObjectInitializer& ObjectInit);

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UPanelWidget> EntryContainer;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UExpressiveTextBlock> DescText;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UObjectiveEntryWidget> EntryWidgetClass;

	void OnEntryHovered(const FObjectiveDbEntry& Data) const;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
