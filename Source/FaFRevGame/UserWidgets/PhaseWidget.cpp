// Copyright (C) RedCraft86. All Rights Reserved.

#include "PhaseWidget.h"
#include "FaFRevSaves.h"
#include "FaFRevSettings.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interfaces/ExitInterface.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "SaveSystem/ToroSaveManager.h"
#include "UserWidgets/ExprTextBlock.h"
#include "FaFRevGameInstance.h"
#include "EnhancedCodeFlow.h"

UGamePhaseGraph* UPhaseNodeWidget::GetGraph()
{
	if (PhaseGraph) return PhaseGraph;
	PhaseGraph = UFaFRevSettings::Get()->PhaseGraph.LoadSynchronous();
	return PhaseGraph;
}

void UPhaseNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (const UFaFRevGlobalSave* Save = UToroSaveManager::GetSaveObject<UFaFRevGlobalSave>(this, SaveTags::TAG_Global))
	{
		SetIsEnabled(Save->PhaseNodes.Contains(PhaseID));
	}
	else
	{
		SetIsEnabled(false);
	}
}

void UPhaseNodeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (const UGamePhaseGraph* Graph = GetGraph(); Graph && NameText)
	{
		if (const UToroDataNode* Node = Graph->GetNodeByID(PhaseID))
		{
			NameText->SetText(Node->Name);
		}
		else
		{
			NameText->SetText(INVTEXT("Invalid Phase"));
		}
	}
}

FReply UPhaseNodeWidget::NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (GetIsEnabled() && MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		ParentWidget->SelectPhase(PhaseID);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

#if WITH_EDITOR
TArray<FString> UPhaseNodeWidget::GetPhaseNames()
{
	TArray<FString> Result;
	if (UGamePhaseGraph* Graph = GetGraph())
	{
		for (const UToroDataNode* Node : Graph->AllNodes)
		{
			Result.Add(Node->Name.ToString());
		}
	}
	return Result;
}

void UPhaseNodeWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, PhaseName))
	{
		if (UGamePhaseGraph* Graph = GetGraph())
		{
			PhaseID = {};
			for (const UToroDataNode* Node : Graph->AllNodes)
			{
				if (Node && Node->Name.ToString().Equals(PhaseName))
				{
					PhaseID = Node->NodeID;
					break;
				}
			}
		}
	}
}
#endif

void UPhaseWidget::SelectPhase(const FGuid& InPhaseID)
{
	if (InPhaseID.IsValid() && SelectedPhase != InPhaseID)
	{
		SelectedPhase = InPhaseID;
		PlayButton->SetIsEnabled(true);
		if (const UGamePhaseGraph* Graph = UFaFRevSettings::Get()->PhaseGraph.LoadSynchronous())
		{
			if (const UToroDataNode* Node = Graph->GetNodeByID(SelectedPhase))
			{
				NameText->SetText(Node->Name);
				DescText->SetText(Node->GetNodeDescription());
			}
		}
	
		const TArray<UPhaseNodeWidget*>& Widgets = GetNodeWidgets();
		for (UPhaseNodeWidget* Widget : Widgets)
		{
			if (Widget) Widget->MarkSelected(Widget->PhaseID == SelectedPhase);
		}

		PlayAnimation(InfoAnim);
	}
}

void UPhaseWidget::OnPlayClicked()
{
	if (SelectedPhase.IsValid())
	{
		PlayAnimationReverse(ActivateAnim);
		SetVisibility(ESlateVisibility::HitTestInvisible);
		UToroShortcutLibrary::StartCameraFade(this, 0.0f);
		FFlow::Delay(this, 1.0f, [this]()
		{
			if (UFaFRevGameInstance* GI = GetGameInstance<UFaFRevGameInstance>())
			{
				GI->PlayFromPhase = SelectedPhase;
				UGameplayStatics::OpenLevelBySoftObjectPtr(this, UToroUtilSettings::Get()->GameplayMap);
			}
		});
	}
}

void UPhaseWidget::InitWidget()
{
	Super::InitWidget();
	PlayButton->SetIsEnabled(false);
	PlayButton->OnClicked.AddDynamic(this, &UPhaseWidget::OnPlayClicked);
	ExitButton->OnClicked.AddDynamic(this, &UPhaseWidget::OnExitClicked);
	
	const TArray<UPhaseNodeWidget*>& Widgets = GetNodeWidgets();
	for (UPhaseNodeWidget* Widget : Widgets)
	{
		if (Widget) Widget->ParentWidget = this;
	}
}

void UPhaseWidget::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	IExitInterface::ReturnToWidget(ParentUI, this);
}
