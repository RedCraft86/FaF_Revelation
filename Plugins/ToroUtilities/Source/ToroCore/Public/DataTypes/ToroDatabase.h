// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "ToroDatabase.generated.h"

UCLASS(Abstract, NotBlueprintable, BlueprintType)
class TOROCORE_API UToroDatabase : public UDataAsset
{
	GENERATED_BODY()
	
public:
#if WITH_EDITOR
	virtual int32 GetTotalData() const { return 0; }
	virtual int32 GetValidData() const { return 0; }
protected:
	virtual void UpdateData() {}
	virtual void PostLoad() override
	{
		Super::PostLoad();
		if (!FApp::IsGame()) UpdateData();
	}
	virtual void PostInitProperties() override
	{
		Super::PostInitProperties();
		if (!FApp::IsGame()) UpdateData();
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		if (!FApp::IsGame()) UpdateData();
	}
#endif
};
