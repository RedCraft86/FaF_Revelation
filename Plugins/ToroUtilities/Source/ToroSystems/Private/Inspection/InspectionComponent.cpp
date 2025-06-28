// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inspection/InspectionComponent.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroWidgetManager.h"
#include "Framework/ToroPlayerState.h"

UInspectionComponent::UInspectionComponent(): TurnRate(1.0, 0.5f), bSecretKnown(false)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

UInspectionComponent* UInspectionComponent::GetInspectionComponent(const UObject* ContextObject, const int32 PlayerIndex)
{
	FIND_PLAYER_COMPONENT(UInspectionComponent)
}

void UInspectionComponent::BeginInspection(AInspectableActor* InInspectable)
{
	if (InInspectable && !Inspectable) // no hot swapping or null setting
	{
		Inspectable = InInspectable;
		SetRelativeRotation(FRotator(0.0f, -10.0f, 0.0f));
		if (UInventoryComponent* Inv = GetInventory())
		{
			Inv->AddArchive(Inspectable->Archive);

			FInvArchiveSlot Slot;
			Inv->GetArchiveSlot(Inspectable->Archive, Slot);
			bSecretKnown = Slot.bKnowSecret;
		}

		if (UInspectionWidget* WidgetObj = GetWidget())
		{
			WidgetObj->ActivateWidget();
			WidgetObj->LoadData(Inspectable->Archive, bSecretKnown);
		}

		if (OnInspection.IsBound()) OnInspection.Execute(true);
	}
}

void UInspectionComponent::EndInspection()
{
	if (Inspectable)
	{
		SetRelativeRotation(FRotator::ZeroRotator);
		if (UInspectionWidget* WidgetObj = GetWidget())
		{
			WidgetObj->DeactivateWidget();
		}

		Inspectable->OnEndInspect();
		Inspectable = nullptr;

		if (OnInspection.IsBound()) OnInspection.Execute(false);
	}
}

void UInspectionComponent::OnMouseXY(const FVector2D& InValue)
{
	if (Inspectable)
	{
		AddLocalRotation(FRotator(InValue.Y * TurnRate.Y, -InValue.X * TurnRate.X, 0.0f));
		if (!bSecretKnown && Inspectable->SeenSecret())
		{
			if (UInventoryComponent* Inv = GetInventory())
			{
				Inv->AddArchive(Inspectable->Archive, true);
			}
			if (UInspectionWidget* WidgetObj = GetWidget())
			{
				WidgetObj->MarkSecretFound();
			}
			bSecretKnown = true;
		}
	}
}

UInspectionWidget* UInspectionComponent::GetWidget()
{
	if (Widget) return Widget;
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		Widget = Manager->FindWidget<UInspectionWidget>();
	}
	return Widget;
}

UInventoryComponent* UInspectionComponent::GetInventory()
{
	if (!Inventory) Inventory = UInventoryComponent::Get(this);
	return Inventory;
}
