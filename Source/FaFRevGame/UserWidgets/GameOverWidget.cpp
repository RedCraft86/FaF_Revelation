// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameOverWidget.h"
#include "Engine/AssetManager.h"
#include "Framework/ToroWidgetManager.h"
#include "Framework/ToroPlayerController.h"
#include "Libraries/ToroGeneralUtils.h"
#include "EnhancedCodeFlow.h"

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 96;
	bAutoActivate = false;
}

void UGameOverWidget::ShowWidget(const FGameplayTag& InCharacter)
{
	if (const FGameOverInfo* Info = GameOverEntries.Find(InCharacter))
	{
		ActivateWidget();
		NameText->SetText(Info->Name);
		DescText->SetText(Info->Description);
		OnShowTutorial(Info->bRightText);
	}
	else
	{
		UToroGeneralUtils::RestartLevel(this);
	}
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
	HideUI->OnClicked.AddDynamic(this, &UGameOverWidget::HideWidget);
	RevealUI->OnClicked.AddDynamic(this, &UGameOverWidget::RevealWidget);
}

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitAnim(SwitchAnim);
}
