// Copyright (C) RedCraft86. All Rights Reserved.

#include "VirtualMouseWidget.h"
#include "Components/CanvasPanelSlot.h"

UVirtualMouse::UVirtualMouse() : bPreviewOnly(true)
{
	Background.ImageSize = FVector2D(16.0f);
	Background.TintColor = FLinearColor::Gray;
	Background.DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	Background.OutlineSettings.Color = FLinearColor::Black;
	Background.OutlineSettings.bUseBrushTransparency = true;
	Background.OutlineSettings.Width = 2.0f;

	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	Padding = {1.0f};
	BrushColor = {1.0f, 1.0f, 1.0f, 0.5f};
	Visibility = ESlateVisibility::HitTestInvisible;
	//RenderTransform.Translation = FVector2D(-8.0f);
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

FVector2D UVirtualMouse::GetPosition() const
{
	return GetCachedGeometry().LocalToAbsolute(FVector2D::ZeroVector);
}

void UVirtualMouse::SimulateClick()
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

void UVirtualMouse::SynchronizeProperties()
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

