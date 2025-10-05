// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "AchievementDatabase.h"
#include "UserInterface/ToroManagedWidget.h"
#include "AchievementWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UAchievementEntryWidget final : public UUserWidget
{
	GENERATED_BODY()

public:

	UAchievementEntryWidget(const FObjectInitializer& ObjectInitializer)
		: UUserWidget(ObjectInitializer)
	{}

	static UAchievementEntryWidget* CreateEntry(UUserWidget* Owner, const FAchievementEntry& Entry);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> IconImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> EntryAnim;

	void InitializeWidget(const FAchievementEntry& Entry);
	void OnAnimationFinished(FWidgetAnimationState& State)
	{
		RemoveFromParent();
	}
};

UCLASS(Abstract)
class TORORUNTIME_API UAchievementWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UAchievementWidget(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{}

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> AchievementList;

	void OnAchievement(const FAchievementEntry& Data);

	virtual void NativeConstruct() override;
};
