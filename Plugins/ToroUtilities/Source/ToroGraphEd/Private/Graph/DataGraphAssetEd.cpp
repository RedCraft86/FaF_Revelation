// Copyright (C) RedCraft86. All Rights Reserved.

#include "Graph/DataGraphAssetEd.h"
#include "Graph/DataGraphCommands.h"
#include "UObject/ObjectSaveContext.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Framework/Commands/GenericCommands.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#include "AutoLayout/DirectLayoutStrategy.h"
#include "AutoLayout/TreeLayoutStrategy.h"
#include "AutoLayout/AutoLayoutBase.h"
#include "Graph/DataGraphToolbar.h"
#include "Nodes/EdDataGraphEdge.h"
#include "Nodes/EdDataGraphNode.h"
#include "Graph/DataGraphSchema.h"
#include "GraphEditorActions.h"
#include "ToroGraphSettings.h"
#include "Graph/EdDataGraph.h"
#include "EdGraphUtilities.h"
#include "EditorClassUtils.h"
#include "ToroGraphEd.h"

#define LOCTEXT_NAMESPACE "ToroGraphEd"

namespace ToroGraphStatics
{
	static const FName EditorAppName(TEXT("DataGraphEditorApp"));
	static const FName PropertyID(TEXT("Property"));
	static const FName ViewportID(TEXT("Viewport"));
}

FDataGraphAssetEd::FDataGraphAssetEd()
{
	EditingGraph = nullptr;
	OnPackageSavedDelegateHandle = UPackage::PackageSavedWithContextEvent.AddRaw(
		this, &FDataGraphAssetEd::OnPackageSavedWithContext);
}

FDataGraphAssetEd::~FDataGraphAssetEd()
{
	UPackage::PackageSavedWithContextEvent.Remove(OnPackageSavedDelegateHandle);
}

void FDataGraphAssetEd::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UDataGraphBase* Graph)
{
	EditingGraph = Graph;
	CreateEdGraph();

	FGenericCommands::Register();
	FGraphEditorCommands::Register();
	FDataGraphCommands::Register();

	if (!ToolbarBuilder.IsValid())
	{
		ToolbarBuilder = MakeShareable(new FDataGraphToolbar(SharedThis(this)));
	}

	BindCommands();
	CreateInternalWidgets();
	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarBuilder->AddDataGraphToolbar(ToolbarExtender);

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_DataGraphEditor_Layout_v2")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.65f)
					->AddTab(ToroGraphStatics::ViewportID, ETabState::OpenedTab)->SetHideTabWell(true)
				)
				->Split
				(
				FTabManager::NewStack()
					->SetSizeCoefficient(0.55f)
					->AddTab(ToroGraphStatics::PropertyID, ETabState::OpenedTab)->SetHideTabWell(true)
				)
			)
		);

	InitAssetEditor(Mode, InitToolkitHost, ToroGraphStatics::EditorAppName,
		StandaloneDefaultLayout, true, true, EditingGraph);

	RegenerateMenusAndToolbars();
}

void FDataGraphAssetEd::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu", "Data Graph Editor"));
	const TSharedRef<FWorkspaceItem> WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	
	InTabManager->RegisterTabSpawner(ToroGraphStatics::ViewportID, FOnSpawnTab::CreateSP(
		this, &FDataGraphAssetEd::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(ToroGraphStatics::PropertyID, FOnSpawnTab::CreateSP(
		this, &FDataGraphAssetEd::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab", "Property"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FDataGraphAssetEd::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner(ToroGraphStatics::ViewportID);
	InTabManager->UnregisterTabSpawner(ToroGraphStatics::PropertyID);
}

void FDataGraphAssetEd::PostRegenerateMenusAndToolbars()
{
	TArray<UObject*> Objs = GetEditingObjects();
	
	TArray<UClass*> ClassList;
	for (const UObject* Obj : Objs)
	{
		check(Obj);
		ClassList.Add(Obj->GetClass());
	}

	const UClass* CommonBaseClass = UClass::FindCommonBase(ClassList);
	const bool bNotAllSame = Objs.Num() > 0 && Objs[0]->GetClass() != CommonBaseClass;

	if (!CommonBaseClass) return;

	if (CommonBaseClass->ClassGeneratedBy)
	{
		const TSharedRef<SHorizontalBox> MenuOverlayBox = SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			.ShadowOffset(FVector2D::UnitVector)
			.Text(NSLOCTEXT("GenericEditor", "BlueprintEditor_ParentClass", "Parent class: "))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SSpacer)
			.Size(FVector2D(2.0f, 1.0f))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.ShadowOffset(FVector2D::UnitVector)
			.Text(FText::FromName(CommonBaseClass->ClassGeneratedBy->GetFName()))
			.TextStyle(FAppStyle::Get(), "Common.InheritedFromBlueprintTextStyle")
			.ToolTipText(NSLOCTEXT("GenericEditor", "ParentClassToolTip", "The class that the current Blueprint is based on. The parent provides the base definition, which the current Blueprint extends."))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.VAlign(VAlign_Center)
			.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
			.OnClicked(this, &FDataGraphAssetEd::OnFindBlueprintParent, CommonBaseClass->ClassGeneratedBy)
			.ToolTipText(NSLOCTEXT("GenericEditor", "FindParentInCBToolTip", "Find parent in Content Browser"))
			.ContentPadding(4.0f)
			.ForegroundColor(FSlateColor::UseForeground())
			[
				SNew(SImage)
				.Image(FAppStyle::GetBrush("Icons.Search"))
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.VAlign(VAlign_Center)
			.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
			.OnClicked(this, &FDataGraphAssetEd::OnEditParentClass, CommonBaseClass->ClassGeneratedBy)
			.ToolTipText(NSLOCTEXT("GenericEditor", "EditParentClassToolTip", "Open parent in editor"))
			.ContentPadding(4.0f)
			.ForegroundColor(FSlateColor::UseForeground())
			[
				SNew(SImage)
				.Image(FAppStyle::GetBrush("Icons.Edit"))
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SSpacer)
			.Size(FVector2D(8.0f, 1.0f))
		];
		SetMenuOverlay(MenuOverlayBox);
	}
	else
	{
		const TSharedRef<SHorizontalBox> MenuOverlayBox = SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			.ShadowOffset(FVector2D::UnitVector)
			.Text(bNotAllSame ? NSLOCTEXT("GenericEditor", "SimpleAssetEditor_AssetType_Varied", "Common Asset Type: ") : LOCTEXT("SimpleAssetEditor_AssetType", "Asset Type: "))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(0.0f, 0.0f, 8.0f, 0.0f)
		[
			FEditorClassUtils::GetSourceLink(CommonBaseClass)
		];
		SetMenuOverlay(MenuOverlayBox);
	}
}

FName FDataGraphAssetEd::GetToolkitFName() const
{
	return FName("FDataGraphEditor");
}

FText FDataGraphAssetEd::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Data Graph Editor");
}

FText FDataGraphAssetEd::GetToolkitName() const
{
	return FText::FromString(EditingGraph->GetName());
}

FText FDataGraphAssetEd::GetToolkitToolTipText() const
{
	return GetToolTipTextForObject(EditingGraph);
}

FLinearColor FDataGraphAssetEd::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

FString FDataGraphAssetEd::GetWorldCentricTabPrefix() const
{
	return TEXT("DataGraphEditor");
}

FString FDataGraphAssetEd::GetDocumentationLink() const
{
	return TEXT("https://github.com/jinyuliao/GenericGraph");
}

void FDataGraphAssetEd::SaveAsset_Execute()
{
	if (EditingGraph)
	{
		RebuildDataGraph();
	}

	FAssetEditorToolkit::SaveAsset_Execute();
}

void FDataGraphAssetEd::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingGraph);
	Collector.AddReferencedObject(EditingGraph->EdGraph);
}

void FDataGraphAssetEd::RegisterToolbarTab(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

TSharedRef<SDockTab> FDataGraphAssetEd::SpawnTab_Viewport(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId() == ToroGraphStatics::ViewportID);
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab).Label(LOCTEXT("ViewportLabel", "Viewport"));
	if (ViewportWidget.IsValid()) SpawnedTab->SetContent(ViewportWidget.ToSharedRef());
	return SpawnedTab;
}

TSharedRef<SDockTab> FDataGraphAssetEd::SpawnTab_Details(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId() == ToroGraphStatics::PropertyID);
	return SNew(SDockTab).Label(LOCTEXT("DetailsLabel", "Property"))[ PropertyWidget.ToSharedRef() ];
}

void FDataGraphAssetEd::CreateInternalWidgets()
{
	ViewportWidget = CreateViewportWidget();

	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(Args);
	PropertyWidget->SetObject(EditingGraph);
	PropertyWidget->OnFinishedChangingProperties().AddSP(
		this,&FDataGraphAssetEd::OnFinishedChangingProperties);
}

TSharedRef<SGraphEditor> FDataGraphAssetEd::CreateViewportWidget()
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_DataGraph", "Data Graph");

	CreateCommandList();

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FDataGraphAssetEd::OnSelectedNodesChanged);

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(EditingGraph->EdGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

void FDataGraphAssetEd::BindCommands()
{
	ToolkitCommands->MapAction(FDataGraphCommands::Get().AutoArrange,
		FExecuteAction::CreateSP(this, &FDataGraphAssetEd::AutoArrange),
		FCanExecuteAction::CreateSP(this, &FDataGraphAssetEd::CanAutoArrange)
	);
}

void FDataGraphAssetEd::CreateEdGraph() const
{
	if (!EditingGraph->EdGraph)
	{
		EditingGraph->EdGraph = CastChecked<UEdDataGraph>(FBlueprintEditorUtils::CreateNewGraph(EditingGraph,
			NAME_None, UEdDataGraph::StaticClass(), UEdDataGraphSchema::StaticClass()));

		EditingGraph->EdGraph->bAllowDeletion = false;
		const UEdGraphSchema* Schema = EditingGraph->EdGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*EditingGraph->EdGraph);
	}
}

void FDataGraphAssetEd::CreateCommandList()
{
	if (GraphEditorCommands.IsValid()) return;
	GraphEditorCommands = MakeShareable(new FUICommandList);

	GraphEditorCommands->MapAction(FDataGraphCommands::Get().AutoArrange,
		FExecuteAction::CreateRaw(this, &FDataGraphAssetEd::AutoArrange),
		FCanExecuteAction::CreateRaw(this, &FDataGraphAssetEd::CanAutoArrange)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateRaw(this, &FDataGraphAssetEd::SelectAllNodes),
		FCanExecuteAction::CreateRaw(this, &FDataGraphAssetEd::CanSelectAllNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FDataGraphAssetEd::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FDataGraphAssetEd::CanDeleteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FDataGraphAssetEd::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FDataGraphAssetEd::CanCopyNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(this, &FDataGraphAssetEd::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FDataGraphAssetEd::CanCutNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(this, &FDataGraphAssetEd::PasteNodes),
		FCanExecuteAction::CreateRaw(this, &FDataGraphAssetEd::CanPasteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(this, &FDataGraphAssetEd::DuplicateNodes),
		FCanExecuteAction::CreateRaw(this, &FDataGraphAssetEd::CanDuplicateNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Rename,
		FExecuteAction::CreateSP(this, &FDataGraphAssetEd::OnRenameNode),
		FCanExecuteAction::CreateSP(this, &FDataGraphAssetEd::CanRenameNodes)
	);
}

void FDataGraphAssetEd::RebuildDataGraph() const
{
	if (EditingGraph == nullptr)
	{
		UE_LOG(LogDataGraphEditor, Warning, TEXT("FDataGraphAssetEditor::RebuildDataGraph EditingGraph is nullptr"));
		return;
	}

	UEdDataGraph* EdGraph = Cast<UEdDataGraph>(EditingGraph->EdGraph);
	check(EdGraph);
	EdGraph->RebuildDataGraph();
}

TSharedPtr<SGraphEditor> FDataGraphAssetEd::GetCurrGraphEditor() const
{
	return ViewportWidget;
}

FGraphPanelSelectionSet FDataGraphAssetEd::GetSelectedNodes() const
{
	return GetCurrGraphEditor().IsValid() ? GetCurrGraphEditor()->GetSelectedNodes() : FGraphPanelSelectionSet{};
}

void FDataGraphAssetEd::SelectAllNodes() const
{
	if (GetCurrGraphEditor().IsValid()) GetCurrGraphEditor()->SelectAllNodes();
}

bool FDataGraphAssetEd::CanSelectAllNodes() const
{
	return true;
}

void FDataGraphAssetEd::DeleteSelectedNodes() const
{
	const TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* EdNode = Cast<UEdGraphNode>(*NodeIt);
		if (EdNode == nullptr || !EdNode->CanUserDeleteNode())
			continue;
		
		if (UEdDataGraphNode* EdNode_Node = Cast<UEdDataGraphNode>(EdNode))
		{
			EdNode_Node->Modify();
			if (const UEdGraphSchema* Schema = EdNode_Node->GetSchema()) Schema->BreakNodeLinks(*EdNode_Node);
			EdNode_Node->DestroyNode();
		}
		else
		{
			EdNode->Modify();
			EdNode->DestroyNode();
		}
	}
}

bool FDataGraphAssetEd::CanDeleteNodes() const
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		const UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanUserDeleteNode())
		{
			return true;
		}
	}

	return false;
}

void FDataGraphAssetEd::DeleteSelectedDuplicateNodes() const
{
	const TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}

	DeleteSelectedNodes();

	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

void FDataGraphAssetEd::CutSelectedNodes() const
{
	CopySelectedNodes();
	DeleteSelectedDuplicateNodes();
}

bool FDataGraphAssetEd::CanCutNodes() const
{
	return CanCopyNodes() && CanDeleteNodes();
}

void FDataGraphAssetEd::CopySelectedNodes() const
{
	FString ExportedText;
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node == nullptr)
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		if (UEdDataGraphEdge* EdNode_Edge = Cast<UEdDataGraphEdge>(*SelectedIter))
		{
			const UEdDataGraphNode* StartNode = EdNode_Edge->GetStartNode();
			const UEdDataGraphNode* EndNode = EdNode_Edge->GetEndNode();
			if (!SelectedNodes.Contains(StartNode) || !SelectedNodes.Contains(EndNode))
			{
				SelectedIter.RemoveCurrent();
				continue;
			}
		}

		Node->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FDataGraphAssetEd::CanCopyNodes() const
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		const UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}

	return false;
}

void FDataGraphAssetEd::PasteNodes() const
{
	if (GetCurrGraphEditor().IsValid()) PasteNodesHere(GetCurrGraphEditor()->GetPasteLocation2f());
}

void FDataGraphAssetEd::PasteNodesHere(const FVector2f& Location) const
{
	const TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid()) return;

	UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph();
	{
		const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
		EdGraph->Modify();

		CurrentGraphEditor->ClearSelectionSet();

		FString TextToImport;
		FPlatformApplicationMisc::ClipboardPaste(TextToImport);

		TSet<UEdGraphNode*> PastedNodes;
		FEdGraphUtilities::ImportNodesFromText(EdGraph, TextToImport, PastedNodes);

		FVector2D AvgNodePosition(0.0f, 0.0f);
		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			const UEdGraphNode* Node = *It;
			AvgNodePosition.X += Node->NodePosX;
			AvgNodePosition.Y += Node->NodePosY;
		}

		const float InvNumNodes = 1.0f / (float)PastedNodes.Num();
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			CurrentGraphEditor->SetNodeSelection(Node, true);
			Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X;
			Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y;
			Node->SnapToGrid(16);
			Node->CreateNewGuid();
		}
	}

	CurrentGraphEditor->NotifyGraphChanged();
	if (UObject* GraphOwner = EdGraph->GetOuter())
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
}

bool FDataGraphAssetEd::CanPasteNodes() const
{
	const TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid()) return false;

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);
	return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FDataGraphAssetEd::DuplicateNodes() const
{
	CopySelectedNodes();
	PasteNodes();
}

bool FDataGraphAssetEd::CanDuplicateNodes() const
{
	return CanCopyNodes();
}

void FDataGraphAssetEd::GraphSettings() const
{
	PropertyWidget->SetObject(EditingGraph);
}

bool FDataGraphAssetEd::CanGraphSettings() const
{
	return true;
}

void FDataGraphAssetEd::AutoArrange() const
{
	UEdDataGraph* EdGraph = Cast<UEdDataGraph>(EditingGraph->EdGraph);
	check(EdGraph);
	
	const FScopedTransaction Transaction(LOCTEXT("DataGraphEditorAutoArrange", "Data Graph Editor: Auto Arrange"));
	
	EdGraph->Modify();
	
	UAutoLayoutBase* LayoutStrategy = nullptr;
	switch (UToroGraphSettings::Get()->AutoLayoutStrategy)
	{
	case EDataGraphLayoutStrategy::Tree:
		LayoutStrategy = NewObject<UAutoLayoutBase>(EdGraph, UTreeLayoutStrategy::StaticClass());
		break;
	case EDataGraphLayoutStrategy::ForceDirected:
		LayoutStrategy = NewObject<UAutoLayoutBase>(EdGraph, UDirectLayoutStrategy::StaticClass());
		break;
	default: break;
	}

	if (LayoutStrategy)
	{
		LayoutStrategy->Layout(EdGraph);
		LayoutStrategy->ConditionalBeginDestroy();
	}
	else
	{
		UE_LOG(LogDataGraphEditor, Error, TEXT("FDataGraphAssetEditor::AutoArrange LayoutStrategy is null."));
	}
}

bool FDataGraphAssetEd::CanAutoArrange() const
{
	return EditingGraph && Cast<UEdDataGraph>(EditingGraph->EdGraph);
}

void FDataGraphAssetEd::OnRenameNode() const
{
	if (GetCurrGraphEditor().IsValid())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			const UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(*NodeIt);
			if (SelectedNode && SelectedNode->bCanRenameNode)
			{
				GetCurrGraphEditor()->IsNodeTitleVisible(SelectedNode, true);
				break;
			}
		}
	}
}

bool FDataGraphAssetEd::CanRenameNodes() const
{
	const UEdDataGraph* EdGraph = Cast<UEdDataGraph>(EditingGraph->EdGraph);
	check(EdGraph);

	const UDataGraphBase* Graph = EdGraph->GetGraph();
	check(Graph)
	
	return GetSelectedNodes().Num() == 1;
}

void FDataGraphAssetEd::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	TArray<UObject*> Selection;
	for (UObject* SelectionEntry : NewSelection)
	{
		Selection.Add(SelectionEntry);
	}

	if (Selection.IsEmpty()) 
	{
		PropertyWidget->SetObject(EditingGraph);

	}
	else
	{
		PropertyWidget->SetObjects(Selection);
	}
}

void FDataGraphAssetEd::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (EditingGraph) EditingGraph->EdGraph->GetSchema()->ForceVisualizationCacheClear();
}

void FDataGraphAssetEd::OnPackageSavedWithContext(const FString& PackageFileName, UPackage* Package, FObjectPostSaveContext ObjectSaveContext)
{
	RebuildDataGraph();
}

FReply FDataGraphAssetEd::OnFindBlueprintParent(TObjectPtr<UObject> SyncToClass) const
{
	if (SyncToClass)
	{
		GEditor->SyncBrowserToObjects({SyncToClass});
	}

	return FReply::Handled();
}

FReply FDataGraphAssetEd::OnEditParentClass(TObjectPtr<UObject> EditClass) const
{
	if (EditClass)
	{
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(EditClass);
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE