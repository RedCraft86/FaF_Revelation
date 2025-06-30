// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameLoadingWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "SaveSystem/ToroSaveManager.h"
#include "FaFRevSaves.h"

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

UGameLoadingWidget::UGameLoadingWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), LoadPercent(0.0f), TotalPackages(0)
{}

void UGameLoadingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	for (auto It = Backgrounds.CreateIterator(); It; ++It)
	{
		if (!ContentTags::IsValidTag(It.Key()) || It.Value().IsNull() || It.Value() == DefaultBackground)
		{
			It.RemoveCurrent();
		}
	}
}

void UGameLoadingWidget::InternalProcessActivation()
{
	TotalPackages = 1;
	LoadPercent = 0.0f;
	LoadBar->SetPercent(0.0f);
	LoadText->SetText(INVTEXT("Initializing!"));

	UTexture2D* ImgPtr = DefaultBackground;
	if (const UFaFRevGlobalSave* Save = UToroSaveManager::GetSaveObject<UFaFRevGlobalSave>(this, SaveTags::TAG_Global))
	{
		TArray<TSoftObjectPtr<UTexture2D>> Images;
		for (const TPair<FGameplayTag, TSoftObjectPtr<UTexture2D>>& Pair : Backgrounds)
		{
			// Make sure the loading background is valid and we have unlocked it
			if (!Pair.Value.IsNull() && Save->Content.Contains(Pair.Key))
			{
				Images.Add(Pair.Value);
			}
		}

		if (!Images.IsEmpty())
		{
			const int32 Idx = Images.Num() == 1 ? 0 : FMath::RandRange(0, Images.Num() - 1);
			ImgPtr = Images[Idx].LoadSynchronous();
		}
	}
	LoadBG->SetBrushFromTexture(ImgPtr, true);
	
	Super::InternalProcessActivation();
}

void UGameLoadingWidget::InternalProcessDeactivation()
{
	TotalPackages = 0;
	LoadPercent = 1.0f;
	LoadBar->SetPercent(1.0f);
	LoadText->SetText(INVTEXT("Loading Complete!"));
	LoadBG->SetBrushFromTexture(DefaultBackground, true);
	Super::InternalProcessDeactivation();
}

void UGameLoadingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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
