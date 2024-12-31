// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailWidgetRow.h"
#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailsCustomization/DetailsHelpers.h"
#include "Inventory/InventoryItemData.h"

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
			GeneralCat.AddProperty(GET_CLASS_PROPERTY(UInventoryItemData, Priority));
			GeneralCat.AddProperty(GET_CLASS_PROPERTY(UInventoryItemData, UniqueID));
			GeneralCat.AddProperty(GET_CLASS_PROPERTY(UInventoryItemData, Thumbnail));
			GeneralCat.AddProperty(GET_CLASS_PROPERTY(UInventoryItemData, DisplayName));
			GeneralCat.AddProperty(GET_CLASS_PROPERTY(UInventoryItemData, Description));
		
			GET_CLASS_PROPERTY_VAR(UInventoryItemData, StackingMode, StackingMode);
			GET_CLASS_PROPERTY_VAR(UInventoryItemData, StackingValue, StackingValue);
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

			GET_CLASS_PROPERTY_VAR(UInventoryItemData, ItemType, Category);
			GET_CLASS_PROPERTY_VAR(UInventoryItemData, CustomType, CategoryValue);
			GeneralCat.AddCustomRow(INVTEXT("Category"))
			.NameContent()
			[
				Category->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.Padding(0.0f, 0.0f, 10.0f, 0.0f)
				.MinWidth(110.0f)
				.AutoWidth()
				[
					Category->CreatePropertyValueWidget()
				]
				+SHorizontalBox::Slot()
				.MinWidth(250.0f)
				.AutoWidth()
				[
					CategoryValue->CreatePropertyValueWidget()
				]
			];
		}

		IDetailCategoryBuilder& MeshCat = DetailBuilder.EditCategory(TEXT("Mesh"));
		{
			GET_CLASS_PROPERTY_VAR(UInventoryItemData, PreviewZoom, PreviewZoom)
			MeshCat.AddCustomRow(INVTEXT("Preview Zoom"))
			.OverrideResetToDefault(FResetToDefaultOverride::Create(
				TAttribute<bool>::CreateLambda([PreviewZoom]() -> bool {
					return PreviewZoom->DiffersFromDefault();
				}),
				FSimpleDelegate::CreateLambda([PreviewZoom]()
				{
					PreviewZoom->ResetToDefault();
				})))
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
						SNew(STextBlock)
						.Text(INVTEXT("Min"))
						.Font(IDetailLayoutBuilder::GetDetailFont())
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
						SNew(STextBlock)
						.Text(INVTEXT("Max"))
						.Font(IDetailLayoutBuilder::GetDetailFont())
					]
					+SHorizontalBox::Slot()
					.MinWidth(75.0f)
					.AutoWidth()
					[
						PreviewZoom->GetChildHandle(1)->CreatePropertyValueWidget()
					]
				]
			];

			GET_CLASS_PROPERTY_VAR(UInventoryItemData, BaseMesh, BaseMesh)
			
			uint32 NumChildren;
			BaseMesh->GetNumChildren(NumChildren);
			for (uint32 i = 0; i < NumChildren; i++)
			{
				MeshCat.AddProperty(BaseMesh->GetChildHandle(i));
			}
		}
	}
};