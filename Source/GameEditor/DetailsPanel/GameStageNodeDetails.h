// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors and RedCraft86

#pragma once

#include "IDetailGroup.h"
#include "DetailWidgetRow.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailCustomization.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"
#include "Factories/DataAssetFactory.h"
#include "AssetToolsModule.h"

#include "GameStage/GameStageData.h"
#include "GameStage/Flow/GameStageNode.h"

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

			UObject* AssetObject = nullptr;
			StageData->GetValue(AssetObject);

			IDetailCategoryBuilder& RootCategory = DetailLayout.EditCategory(TEXT("Stage"));
			RootCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UGameStageNode, Requirements)));

			IDetailGroup& Group = RootCategory.AddGroup(TEXT("StageData"), INVTEXT("Data"));
			Group.HeaderRow()
			.NameContent() 
			[
				StageData->CreatePropertyNameWidget()
			]
			.ValueContent()
			.HAlign(HAlign_Fill)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					StageData->CreatePropertyValueWidget()
				]
				+SVerticalBox::Slot()
				.Padding(0.0f, 2.0f, 0.0f, 2.0f)
				.AutoHeight()
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.Text(INVTEXT("New Asset"))
					.ToolTipText(INVTEXT("Create a new Game Stage Data asset."))
					.OnClicked_Lambda([StageData]()
					{
						IAssetTools& Tools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
						const FString AssetPath(TEXT("/Game/Data/GameStage/GS_"));
						
						FString UniquePackageName, UniqueAssetName;
						Tools.CreateUniqueAssetName(AssetPath, TEXT("0"), UniquePackageName, UniqueAssetName);

						UDataAssetFactory* Factory = NewObject<UDataAssetFactory>();
						Factory->DataAssetClass = UGameStageData::StaticClass();

						if (UObject* NewAsset = Tools.CreateAssetWithDialog(
							UniqueAssetName, FPackageName::GetLongPackagePath(TEXT("/Game/Data/")),
							UGameStageData::StaticClass(), Factory, NAME_None, false))
						{
							StageData->SetValue(NewAsset);
						}

						return FReply::Handled();
					})
				]
			];

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

				Group.AddWidgetRow()
				[
					SNew(SBox)
					.Padding(FMargin(-16.0f, 2.0f, 0.0f, 2.0f)) 
					[
						ObjectView
					]
				];
			}
		}
	}
};
