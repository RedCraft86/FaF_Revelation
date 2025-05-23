// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailWidgetRow.h"
#include "SGameplayTagCombo.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsPanels/CustomizationHelpers.h"
#include "DataTypes/PlayerLockFlag.h"

#define STRUCTNAME FPlayerLockFlag
class FPlayerLockDetails final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FPlayerLockDetails>();
	}

private:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		STRUCT_PROPERTY_VAR_NS(LockTag, LockTag);
		STRUCT_PROPERTY_VAR_NS(LockKey, LockKey);
		
		TArray<const void*> StructPtrs;
		StructPropertyHandle->AccessRawData(StructPtrs);
		if (StructPtrs.IsEmpty()) return;
		if (StructPtrs.Num() > 1)
		{
			HeaderRow.NameContent()
			[
				StructPropertyHandle->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				GENERIC_LABEL(Multiple Selected)
			];
			return;
		}

		if (FPlayerLockFlag* StructPtr = (FPlayerLockFlag*)StructPtrs[0])
		{
			if (!StructPtr->LockTag.IsValid())
			{
				StructPropertyHandle->NotifyPreChange();
				StructPtr->ResetTag();
				StructPropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
			}
			
			HeaderRow.NameContent()
			[
				StructPropertyHandle->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.Padding(0.0f, 2.0f, 0.0f, 2.0f)
				.AutoHeight()
				[
					SNew(SGameplayTagCombo).PropertyHandle(LockTag)
				]
				+SVerticalBox::Slot()
				.Padding(0.0f, 2.0f, 0.0f, 2.0f)
				.AutoHeight()
				[
					SNew(SBox)
					.HAlign(HAlign_Fill)
					.Visibility_Lambda([StructPtr]() -> EVisibility
					{
						return !StructPtr || StructPtr->HasValidTag()
							? EVisibility::Collapsed : EVisibility::Visible;
					})
					.Content()
					[
						LockKey->CreatePropertyValueWidget()
					]
				]
			];
		}
	}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {}
};
#undef STRUCTNAME