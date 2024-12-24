// Copyright (C) RedCraft86. All Rights Reserved.

#include "SDataGraphEdge.h"

#include "ConnectionDrawingPolicy.h"
#include "DataGraphEdEdge.h"
#include "DataGraphEdNode.h"

#include "DataEdgeBase.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

#define LOCTEXT_NAMESPACE "DataGraphEditor"

void SDataGraphEdge::Construct(const FArguments& InArgs, UDataGraphEdEdge* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

void SDataGraphEdge::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	const TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);
	
	ContentScale.Bind(this, &SGraphNode::GetContentScale);
	GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(FAppStyle::GetBrush("Graph.TransitionNode.ColorSpill"))
				.ColorAndOpacity(this, &SDataGraphEdge::GetEdgeColor)
			]
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(FAppStyle::GetBrush("Graph.TransitionNode.Icon"))
				.Visibility(this, &SDataGraphEdge::GetEdgeImageVisibility)
			]

			+ SOverlay::Slot()
			.Padding(FMargin(4.0f, 4.0f, 4.0f, 4.0f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SAssignNew(InlineEditableText, SInlineEditableTextBlock)
					.ColorAndOpacity(FLinearColor::Black)
					.Visibility(this, &SDataGraphEdge::GetEdgeTitleVisibility)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
					.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
					.OnTextCommitted(this, &SDataGraphEdge::OnNameCommitted)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					NodeTitle.ToSharedRef()
				]
				
			]
		];
}

bool SDataGraphEdge::RequiresSecondPassLayout() const
{
	return true;
}

void SDataGraphEdge::PerformSecondPassLayout(const TMap<UObject*, TSharedRef<SNode>>& NodeToWidgetLookup) const
{
	FGeometry StartGeom, EndGeom;
	UDataGraphEdEdge* EdgeNode = CastChecked<UDataGraphEdEdge>(GraphNode);
	const UDataGraphEdNode* Start = EdgeNode->GetStartNode();
	const UDataGraphEdNode* End = EdgeNode->GetEndNode();
	if (Start && End)
	{
		const TSharedRef<SNode>* pFromWidget = NodeToWidgetLookup.Find(Start);
		const TSharedRef<SNode>* pToWidget = NodeToWidgetLookup.Find(End);
		if (pFromWidget && pToWidget)
		{
			StartGeom = FGeometry(FVector2D(Start->NodePosX, Start->NodePosY),
				FVector2D::ZeroVector, (*pFromWidget)->GetDesiredSize(), 1.0f);
			
			EndGeom = FGeometry(FVector2D(End->NodePosX, End->NodePosY),
				FVector2D::ZeroVector, (*pToWidget)->GetDesiredSize(), 1.0f);
		}
	}

	PositionBetweenTwoNodesWithOffset(StartGeom, EndGeom, 0, 1);
}

void SDataGraphEdge::PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const
{
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;
	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	constexpr float Height = 30.0f;
	const FVector2D DesiredNodeSize = GetDesiredSize();
	FVector2D DeltaPos(EndAnchorPoint - StartAnchorPoint);
	if (DeltaPos.IsNearlyZero())
	{
		DeltaPos = FVector2D(10.0f, 0.0f);
	}

	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();
	const FVector2D NewCenter = StartAnchorPoint + (0.5f * DeltaPos) + (Height * Normal);
	const FVector2D DeltaNormal = DeltaPos.GetSafeNormal();
	
	constexpr float MultiNodeSpace = 0.2f;
	constexpr float MultiNodeStep = 1.f + MultiNodeSpace;
	const float MultiNodeStart = -((MaxNodes - 1) * MultiNodeStep) / 2.f;
	const float MultiNodeOffset = MultiNodeStart + (NodeIndex * MultiNodeStep);

	const FVector2D NewCorner = NewCenter - 0.5f * DesiredNodeSize +
		DeltaNormal * MultiNodeOffset * DesiredNodeSize.Size();
	
	GraphNode->NodePosX = NewCorner.X;
	GraphNode->NodePosY = NewCorner.Y;
}

void SDataGraphEdge::OnNameCommitted(const FText& InText, ETextCommit::Type CommitInfo)
{
	OnNameTextCommited(InText, CommitInfo);
	UDataGraphEdEdge* MyNode = CastChecked<UDataGraphEdEdge>(GraphNode);
	if (MyNode && MyNode->GraphEdge)
	{
		const FScopedTransaction Transaction(LOCTEXT("RenameEdge", "Data Graph Editor: Rename Edge"));
		MyNode->Modify();
		MyNode->GraphEdge->Modify();
		MyNode->GraphEdge->SetNodeTitle(InText);
		UpdateGraphNode();
	}
}

FSlateColor SDataGraphEdge::GetEdgeColor() const
{
	const UDataGraphEdEdge* Node = CastChecked<UDataGraphEdEdge>(GraphNode);
	return Node && Node->GraphEdge ? Node->GraphEdge->EdgeColor : FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);
}

EVisibility SDataGraphEdge::GetEdgeImageVisibility() const
{
	const UDataGraphEdEdge* Node = CastChecked<UDataGraphEdEdge>(GraphNode);
	return Node && Node->GraphEdge && Node->GraphEdge->bShouldDrawTitle ? EVisibility::Hidden : EVisibility::Visible;
}

EVisibility SDataGraphEdge::GetEdgeTitleVisibility() const
{
	const UDataGraphEdEdge* Node = CastChecked<UDataGraphEdEdge>(GraphNode);
	if (Node && Node->GraphEdge && Node->GraphEdge->bShouldDrawTitle) return EVisibility::Visible;
	return EVisibility::Collapsed;
}

#undef LOCTEXT_NAMESPACE
