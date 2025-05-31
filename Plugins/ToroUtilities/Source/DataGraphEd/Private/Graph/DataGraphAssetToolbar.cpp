// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataGraphAssetToolbar.h"
#include "DataGraphCommands.h"
#include "DataGraphAssetEditor.h"
#include "DataGraphEd.h"

#define LOCTEXT_NAMESPACE "DataGraphEditor"

void FDataGraphAssetToolbar::AddDataGraphToolbar(TSharedPtr<FExtender> Extender)
{
	check(DataGraphEditor.IsValid());
	const TSharedPtr<FDataGraphAssetEditor> DataGraphEditorPtr = DataGraphEditor.Pin();

	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After,
		DataGraphEditorPtr->GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP(this, &FDataGraphAssetToolbar::FillDataGraphToolbar));
	
	DataGraphEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FDataGraphAssetToolbar::FillDataGraphToolbar(FToolBarBuilder& ToolbarBuilder) const
{
	check(DataGraphEditor.IsValid());
	TSharedPtr<FDataGraphAssetEditor> DataGraphEditorPtr = DataGraphEditor.Pin();

	ToolbarBuilder.BeginSection("Util");
	{
		ToolbarBuilder.AddToolBarButton(FDataGraphCommands::Get().AutoArrange,
			NAME_None,
			LOCTEXT("AutoArrange_Label", "Auto Arrange"),
			LOCTEXT("AutoArrange_ToolTip", "Auto arrange nodes, not perfect, but still handy"),
			FSlateIcon(FDataGraphEdStyle::GetStyleSetName(), "DataGraph.AutoArrangeNodes"));
	}
	ToolbarBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE
