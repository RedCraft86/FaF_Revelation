// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "ArchiveWidget.generated.h"

class UImage;
class UBorder;
class UButton;
class UTextBlock;
class UExpressiveTextBlock;

UCLASS(Abstract, Blueprintable, BlueprintType)
class UArchiveEntryWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class UArchiveWidget;

public:

	UArchiveEntryWidget(const FObjectInitializer& ObjectInit);

	DECLARE_DELEGATE_OneParam(FOnEntrySelected, const UArchiveEntryWidget*);
	FOnEntrySelected OnEntrySelected;

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UTextBlock> NameText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UImage> IconImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UButton> SelectButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UBorder> SelectBorder;

	UFUNCTION() void OnSelectClicked() const;

	void InitEntry(const FGameplayTag& Key) const;
	void SetSelectionState(const bool bSelected) const;

	virtual void NativeConstruct() override;
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class UArchiveWidget final : public UToroUserWidget
{
	GENERATED_BODY()

public:

	UArchiveWidget(const FObjectInitializer& ObjectInit);

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UPanelWidget> EntryContainer;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UExpressiveTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UImage> ContentImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UExpressiveTextBlock> ContentText;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UArchiveEntryWidget> EntryWidgetClass;

	UPROPERTY(Transient)
		TMap<FGameplayTag, TObjectPtr<UArchiveEntryWidget>> Entries;

	void DisplayArchive(const FGameplayTag& Key) const;
	void OnEntrySelected(const UArchiveEntryWidget* Entry) const;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
