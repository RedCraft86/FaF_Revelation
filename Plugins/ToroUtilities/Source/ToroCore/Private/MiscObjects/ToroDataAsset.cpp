// Copyright (C) RedCraft86. All Rights Reserved.

#include "MiscObjects/ToroDataAsset.h"

#if WITH_EDITOR
void UToroDataAsset::PostLoad()
{
	Super::PostLoad();
	if (!FApp::IsGame())
	{
		UpdateData();
	}
}

void UToroDataAsset::PostInitProperties()
{
	Super::PostInitProperties();
	if (!FApp::IsGame())
	{
		UpdateData();
	}
}

void UToroDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!FApp::IsGame())
	{
		UpdateData();
	}
}

FText UToroDatabase::GetDescription() const
{
	const FIntPoint Data = GetDataResults();
	return FText::Format(INVTEXT("All: {0}\nValid: {1}\nInvalid: {2}"),
		Data.GetMax(), Data.GetMin(), Data.GetMax() - Data.GetMin());
}
#endif