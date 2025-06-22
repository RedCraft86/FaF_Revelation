// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

class FDataGraphAssetEd;

class FDataGraphToolbar : public TSharedFromThis<FDataGraphToolbar>
{
public:
	
	explicit FDataGraphToolbar(const TSharedPtr<FDataGraphAssetEd>& InDataGraphEditor)
		: DataGraphEditor(InDataGraphEditor)
	{}

	void AddDataGraphToolbar(TSharedPtr<FExtender> Extender);

private:
	
	void FillDataGraphToolbar(FToolBarBuilder& ToolbarBuilder) const;
	TWeakPtr<FDataGraphAssetEd> DataGraphEditor;
};
