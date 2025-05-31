// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

class FDataGraphAssetEditor;

class FDataGraphAssetToolbar : public TSharedFromThis<FDataGraphAssetToolbar>
{
public:
	
	explicit FDataGraphAssetToolbar(const TSharedPtr<FDataGraphAssetEditor>& InDataGraphEditor)
		: DataGraphEditor(InDataGraphEditor) {}

	void AddDataGraphToolbar(TSharedPtr<FExtender> Extender);

private:
	
	void FillDataGraphToolbar(FToolBarBuilder& ToolbarBuilder) const;
	TWeakPtr<FDataGraphAssetEditor> DataGraphEditor;
};