// Copyright (C) RedCraft86. All Rights Reserved.

#include "Graph/DataGraphToolbar.h"
#include "Graph/DataGraphCommands.h"
#include "Graph/DataGraphAssetEd.h"
#include "ToroGraphEd.h"

#define LOCTEXT_NAMESPACE "ToroGraphEd"

void FDataGraphToolbar::AddDataGraphToolbar(TSharedPtr<FExtender> Extender)
{
	check(DataGraphEditor.IsValid());
	const TSharedPtr<FDataGraphAssetEd> DataGraphEditorPtr = DataGraphEditor.Pin();

	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After,
		DataGraphEditorPtr->GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP(this, &FDataGraphToolbar::FillDataGraphToolbar));
	
	DataGraphEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FDataGraphToolbar::FillDataGraphToolbar(FToolBarBuilder& ToolbarBuilder) const
{
	check(DataGraphEditor.IsValid());
	TSharedPtr<FDataGraphAssetEd> DataGraphEditorPtr = DataGraphEditor.Pin();

	ToolbarBuilder.BeginSection("Util");
	{
		ToolbarBuilder.AddToolBarButton(FDataGraphCommands::Get().AutoArrange,
			NAME_None,
			LOCTEXT("AutoArrange_Label", "Auto Arrange"),
			LOCTEXT("AutoArrange_ToolTip", "Auto arrange nodes, not perfect, but still handy"),
			FSlateIcon(FToroGraphEdStyle::GetStyleSetName(), "DataGraph.AutoArrangeNodes"));
	}
	ToolbarBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE