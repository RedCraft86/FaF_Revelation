// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/Border.h"
#include "VirtualMouseWidget.generated.h"

UCLASS()
class FAF_REV_API UVirtualMouse : public UBorder
{
	GENERATED_BODY()

public:

	UVirtualMouse();

	/** If enabled, the widget will be hidden in-game (Zero Opacity) but not collapsed. */
	UPROPERTY(EditAnywhere, Category = "Appearance", AdvancedDisplay)
		uint8 bPreviewOnly:1;

	UFUNCTION(BlueprintCallable, Category = "VirtualMouse")
		void SimulateClick();

protected:
	
	virtual void SynchronizeProperties() override;
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override { return FText::FromString("Input"); }
private: TObjectPtr<UPanelSlot> CachedSlot = nullptr;
#endif
};
