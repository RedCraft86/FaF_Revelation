// Copyright (C) RedCraft86. All Rights Reserved.

#include "LoadingWidgetBase.h"

FText GetLoadStatus(const float Percent)
{
	if (FMath::IsNearlyEqual(Percent, 1.0f))
	{
		return INVTEXT("Loading Complete");
	}
	if (FMath::IsNearlyZero(Percent))
	{
		return INVTEXT("Collecting Packages");
	}
	return INVTEXT("Loading Packages");
}

ULoadingWidgetBase::ULoadingWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), LoadPercent(0.0f), TotalPackages(0)
{
	ZOrder = 49;
	bAutoActivate = false;
}

void ULoadingWidgetBase::InternalProcessActivation()
{
	TotalPackages = 1;
	LoadPercent = 0.0f;
	LoadBar->SetPercent(0.0f);
	LoadText->SetText(INVTEXT("Initializing!"));
	Super::InternalProcessActivation();
}

void ULoadingWidgetBase::InternalProcessDeactivation()
{
	TotalPackages = 0;
	LoadPercent = 1.0f;
	LoadBar->SetPercent(1.0f);
	LoadText->SetText(INVTEXT("Loading Complete!"));
	Super::InternalProcessDeactivation();
}

void ULoadingWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (IsActivated())
	{
		const int32 Current = GetNumAsyncPackages();
		if (Current > TotalPackages) TotalPackages = Current;
		const int32 NumLeft = TotalPackages - Current;
		LoadPercent = TotalPackages > 0 ? FMath::Clamp((float)NumLeft / TotalPackages, 0.0f, 1.0f) : 1.0f;

		LoadBar->SetPercent(LoadPercent);
		LoadText->SetText(FText::Format(INVTEXT("{0}: {1}% [{2}/{3}]"),
			GetLoadStatus(LoadPercent), FMath::Floor(LoadPercent * 100.0f), NumLeft, TotalPackages));
	}
}
