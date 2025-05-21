// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/Border.h"
#include "VirtualPointer.generated.h"

UCLASS()
class TOROCORE_API UVirtualPointer final : public UBorder
{
	GENERATED_BODY()

public:

	UVirtualPointer();

	/** If enabled, the widget will be hidden in-game (Zero Opacity) but not collapsed. */
	UPROPERTY(EditAnywhere, Category = Appearance, AdvancedDisplay)
		bool bPreviewOnly;

	UFUNCTION(BlueprintCallable, Category = VirtualMouse)
		FVector2D GetPosition() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = VirtualMouse)
		void SimulateClick() const;

protected:
	
	virtual void SynchronizeProperties() override;
#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient) TObjectPtr<UPanelSlot> CachedSlot = nullptr;
#endif
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override { return NSLOCTEXT("UMG", "Input", "Input"); }
#endif
};
