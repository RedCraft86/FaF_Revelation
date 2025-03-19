// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailWidgetRow.h"
#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailsCustomization/DetailsHelpers.h"
#include "Inventory/InventoryItemData.h"

#define CLASSNAME UInventoryItemData
class FInventoryItemCustomization final : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FInventoryItemCustomization>();
	}

private:

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		IDetailCategoryBuilder& GeneralCat = DetailBuilder.EditCategory(TEXT("General"));
		{
			GeneralCat.AddProperty(GET_PROPERTY(Priority));
			GeneralCat.AddProperty(GET_PROPERTY(Thumbnail));
			GeneralCat.AddProperty(GET_PROPERTY(DisplayName));
			GeneralCat.AddProperty(GET_PROPERTY(Description));
		
			GET_PROPERTY_VAR(StackingMode, StackingMode);
			GET_PROPERTY_VAR(StackingValue, StackingValue);
			GeneralCat.AddCustomRow(INVTEXT("Stacking"))
			.NameContent()
			[
				StackingMode->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 10.0f, 0.0f)
				.MinWidth(110.0f)
				.AutoWidth()
				[
					StackingMode->CreatePropertyValueWidget()
				]
				+SHorizontalBox::Slot()
				.MinWidth(50.0f)
				.AutoWidth()
				[
					StackingValue->CreatePropertyValueWidget()
				]
			];

			GET_PROPERTY_VAR(ItemType, ItemType);
			GET_PROPERTY_VAR(CustomType, CustomType);
			GeneralCat.AddCustomRow(INVTEXT("Category"))
			.NameContent()
			[
				ItemType->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 10.0f, 0.0f)
				.MinWidth(110.0f)
				.AutoWidth()
				[
					ItemType->CreatePropertyValueWidget()
				]
				+SHorizontalBox::Slot()
				.MinWidth(250.0f)
				.AutoWidth()
				[
					CustomType->CreatePropertyValueWidget()
				]
			];
		}

		IDetailCategoryBuilder& MeshCat = DetailBuilder.EditCategory(TEXT("Mesh"));
		{
			GET_PROPERTY_VAR(PreviewZoom, PreviewZoom)
			MeshCat.AddCustomRow(INVTEXT("Preview Zoom"))
			.OverrideResetToDefault(SIMPLE_RESET_TO_DEFAULT(PreviewZoom))
			.NameContent()
			[
				PreviewZoom->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 15.0f, 0.0f)
				.AutoWidth()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.Padding(0.0f, 0.0f, 5.0f, 0.0f)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						GENERIC_LABEL(Min)
					]
					+SHorizontalBox::Slot()
					.MinWidth(75.0f)
					.AutoWidth()
					[
						PreviewZoom->GetChildHandle(0)->CreatePropertyValueWidget()
					]
				]
				+SHorizontalBox::Slot()
				.MinWidth(50.0f)
				.AutoWidth()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.Padding(0.0f, 0.0f, 5.0f, 0.0f)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						GENERIC_LABEL(Max)
					]
					+SHorizontalBox::Slot()
					.MinWidth(75.0f)
					.AutoWidth()
					[
						PreviewZoom->GetChildHandle(1)->CreatePropertyValueWidget()
					]
				]
			];

			GET_PROPERTY_VAR(BaseMesh, BaseMesh)
			
			uint32 NumChildren;
			BaseMesh->GetNumChildren(NumChildren);
			for (uint32 i = 0; i < NumChildren; i++)
			{
				MeshCat.AddProperty(BaseMesh->GetChildHandle(i));
			}
		}
	}
};
#undef CLASSNAME