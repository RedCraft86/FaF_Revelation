// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/VirtualPointer.h"
#include "Components/CanvasPanelSlot.h"

UVirtualPointer::UVirtualPointer() : bPreviewOnly(false)
{
	Background.ImageSize = FVector2D(16.0f);
	Background.TintColor = FLinearColor::Gray;
	Background.DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	Background.OutlineSettings.Color = FLinearColor::Black;
	Background.OutlineSettings.bUseBrushTransparency = true;
	Background.OutlineSettings.Width = 2.0f;

	Padding = {1.0f};
	BrushColor = {1.0f, 1.0f, 1.0f, 0.5f};
	Visibility = ESlateVisibility::HitTestInvisible;
}

FVector2D UVirtualPointer::GetPosition() const
{
	return GetCachedGeometry().LocalToAbsolute(FVector2D::ZeroVector);
}

void UVirtualPointer::SimulateClick() const
{
	FSlateApplication& SlateApp = FSlateApplication::Get();

	const FVector2D Pos = GetPosition();
	const FPointerEvent MouseEvent(0, SlateApp.CursorPointerIndex, Pos,
		Pos, SlateApp.GetPressedMouseButtons(), EKeys::LeftMouseButton,
		0, SlateApp.GetPlatformApplication()->GetModifierKeys()
	);

	SlateApp.ProcessMouseButtonDownEvent(nullptr, MouseEvent);
	SlateApp.ProcessMouseButtonUpEvent(MouseEvent);
}

void UVirtualPointer::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (FApp::IsGame())
	{
		if (bPreviewOnly || GetVisibility() == ESlateVisibility::Collapsed) // We cannot be Collapsed either.
		{
			SetRenderOpacity(0.0f);
			SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
#if WITH_EDITOR
	if (CachedSlot != Slot)
	{
		CachedSlot = Slot;
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
		{
			CanvasSlot->SetSize(FVector2D(20.0f));
		}
	}
#endif
}
