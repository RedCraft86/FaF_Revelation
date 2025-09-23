// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserInterface/ToroContainerWidget.h"
#include "UserInterface/ToroManagedWidget.h"

void UToroContainerWidget::ClearNullEntries()
{
	ManagedWidgets.RemoveAll([](const TWeakObjectPtr<UToroManagedWidget>& Elem)
	{
		return !Elem.IsValid();
	});
}

bool UToroContainerWidget::HasWidget(UToroManagedWidget* Widget) const
{
	if (!Widget) return false;
	return ManagedWidgets.ContainsByPredicate([Widget](const TWeakObjectPtr<UToroManagedWidget>& Elem)
	{
		return Elem.IsValid() && Elem.Get() == Widget;
	});
}

bool UToroContainerWidget::PushEntry(UToroManagedWidget* Widget)
{
	ClearNullEntries();
	if (Widget && !HasWidget(Widget))
	{
		ManagedWidgets.Add(Widget);
		return true;
	}
	return false;
}

bool UToroContainerWidget::PopEntry(UToroManagedWidget* Widget)
{
	ClearNullEntries();
	if (Widget && HasWidget(Widget))
	{
		ManagedWidgets.Remove(Widget);
		return true;
	}
	return false;
}

void UToroContainerWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	AddToViewport(ZOrder);
}

bool UToroOverlayContainer::PushEntry(UToroManagedWidget* Widget)
{
	if (Super::PushEntry(Widget))
	{
		WidgetStack->AddChild(Widget);
		return true;
	}
	return false;
}

bool UToroOverlayContainer::PopEntry(UToroManagedWidget* Widget)
{
	if (Super::PopEntry(Widget))
	{
		WidgetStack->RemoveChild(Widget);
		return true;
	}
	return false;
}

bool UToroStackContainer::PushEntry(UToroManagedWidget* Widget)
{
	if (Super::PushEntry(Widget))
	{
		WidgetStack->AddWidgetInstance(*Widget);
		return true;
	}
	return false;
}

bool UToroStackContainer::PopEntry(UToroManagedWidget* Widget)
{
	if (Super::PopEntry(Widget))
	{
		WidgetStack->RemoveWidget(*Widget);
		return true;
	}
	return false;
}
