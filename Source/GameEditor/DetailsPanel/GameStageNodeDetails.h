// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors and RedCraft86

#pragma once

#include "IDetailGroup.h"
#include "DetailWidgetRow.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailCustomization.h"
#include "DetailCustomizations/FlowDataPinValueOwnerCustomization.h"
#include "GameStage/Flow/GameStageNode.h"
#include "Widgets/Layout/SBox.h"

class FGameStageNodeDetails final : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FGameStageNodeDetails());
	}

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override
	{
		const TSharedRef<IPropertyHandle> StageData = DetailLayout.GetProperty(
			GET_MEMBER_NAME_CHECKED(UGameStageNode, StageData));
		if (StageData->IsValidHandle())
		{
			StageData->MarkHiddenByCustomization();
			StageData->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([&DetailLayout]()
			{
				DetailLayout.ForceRefreshDetails();
			}));

			IDetailCategoryBuilder& RootCategory = DetailLayout.EditCategory(TEXT("Stage"));
			RootCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UGameStageNode, Requirements)));

			IDetailGroup& Group = RootCategory.AddGroup(TEXT("StageData"), INVTEXT("Data"));
			Group.HeaderProperty(StageData);

			UObject* AssetObject = nullptr;
			StageData->GetValue(AssetObject);
			if (AssetObject)
			{
				FDetailsViewArgs Args;
				Args.bAllowSearch = false;
				Args.bHideSelectionTip = true;
				Args.bLockable = false;
				Args.bShowOptions = false;
				Args.bUpdatesFromSelection = false;
				Args.bShowPropertyMatrixButton = false;
				Args.bShowObjectLabel = false;
				Args.NameAreaSettings = FDetailsViewArgs::HideNameArea;
				Args.ColumnWidth = 0.5f;

				const TSharedRef<IDetailsView> ObjectView = FModuleManager::GetModulePtr
					<FPropertyEditorModule>("PropertyEditor")->CreateDetailView(Args);

				ObjectView->SetObject(AssetObject);
				ObjectView->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateLambda([](const FPropertyAndParent&)
				{
					return true;
				}));

				Group.AddWidgetRow()
				[
					SNew(SBox)
					.Padding(FMargin(-16.0f, 2.0f, -2.0f, 2.0f)) 
					[
						ObjectView
					]
				];
			}
		}
	}
};
