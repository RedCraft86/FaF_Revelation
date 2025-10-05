// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Tutorials/TutorialWidget.h"

void UTutorialEntryWidget::InitializeWidget(const FTutorialEntry& Entry) const
{
	PreviewImage->SetBrushFromTexture(Entry.Image.LoadSynchronous());
	ContentText->SetText(Entry.Description);
	TitleText->SetText(Entry.Name);
}

UTutorialWidget::UTutorialWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
}

void UTutorialWidget::ShowTutorial(const FTutorialEntry& Entry)
{
	CreateEntry(Entry);
	if (!IsPushed())
	{
		PushWidget();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UTutorialWidget::MoveLeft()
{
	const int32 NextIdx = EntryContainer->GetActiveWidgetIndex() - 1;
	if (NextIdx > 0)
	{
		EntryContainer->SetActiveWidgetIndex(NextIdx);
		UpdatePageCount();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UTutorialWidget::MoveRight()
{
	const int32 NextIdx = EntryContainer->GetActiveWidgetIndex() + 1;
	if (NextIdx < EntryContainer->GetNumWidgets())
	{
		EntryContainer->SetActiveWidgetIndex(NextIdx);
		UpdatePageCount();
	}
}

void UTutorialWidget::UpdatePageCount() const
{
	PageCount->SetText(FText::Format(INVTEXT("{0}/{1}"),
		EntryContainer->GetActiveWidgetIndex() + 1,
		EntryContainer->GetNumWidgets()));
}

void UTutorialWidget::CreateEntry(const FTutorialEntry& Entry)
{
	if (UTutorialEntryWidget* Widget = CreateWidget<UTutorialEntryWidget>(this, EntryClass))
	{
		EntryContainer->AddChild(Widget);
		Widget->InitializeWidget(Entry);
		UpdatePageCount();
	}
}

void UTutorialWidget::PopWidget()
{
	Super::PopWidget();
	EntryContainer->ClearChildren();
	if (CloseFunc) CloseFunc();
}

void UTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddUniqueDynamic(this, &UTutorialWidget::CloseMenu);
	LeftButton->OnClicked.AddUniqueDynamic(this, &UTutorialWidget::MoveLeft);
	RightButton->OnClicked.AddUniqueDynamic(this, &UTutorialWidget::MoveRight);
}
