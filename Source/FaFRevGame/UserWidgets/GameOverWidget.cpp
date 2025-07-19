// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameOverWidget.h"
#include "Engine/AssetManager.h"
#include "Framework/ToroPlayerController.h"
#include "Libraries/ToroGeneralUtils.h"
#include "EnhancedCodeFlow.h"

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 96;
	bAutoActivate = false;
}

void UGameOverWidget::Restart()
{
	FadeOut([this]()
	{
		UToroGeneralUtils::RestartLevel(this);
	});
}

void UGameOverWidget::MainMenu()
{
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(UToroUtilSettings::Get()->LaunchMap.ToSoftObjectPath());
	FadeOut([this]()
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, UToroUtilSettings::Get()->LaunchMap);
	});
}

void UGameOverWidget::FadeOut(const TFunction<void()>& Callback)
{
	PlayAnimationReverse(ActivateAnim);
	if (Controller->PlayerCameraManager)
	{
		Controller->PlayerCameraManager->StartCameraFade(0.0f, 1.0f,
			1.0f, FLinearColor::Black, true, true);
	}

	FFlow::Delay(this, 1.0f, Callback);
}

void UGameOverWidget::InitWidget()
{
	Super::InitWidget();
	Controller = AToroPlayerController::Get(this);
	RestartBtn->OnClicked.AddDynamic(this, &UGameOverWidget::Restart);
	MainMenuBtn->OnClicked.AddDynamic(this, &UGameOverWidget::MainMenu);
}
