// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ContentBrowserFrontEndFilterExtension.h"
#include "DuplicateAssetFilter.generated.h"

UCLASS()
class TOROEDITOR_API UDuplicateAssetFilter final : public UContentBrowserFrontEndFilterExtension
{
	GENERATED_BODY()

public:

	virtual void AddFrontEndFilterExtensions(TSharedPtr<FFrontendFilterCategory> DefaultCategory,
		TArray<TSharedRef<FFrontendFilter>>& InOutFilterList) const override;
};
