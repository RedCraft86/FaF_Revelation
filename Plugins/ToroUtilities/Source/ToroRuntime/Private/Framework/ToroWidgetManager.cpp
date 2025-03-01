// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroWidgetManager.h"
#include "UserWidgets/MessageWidgetBase.h"
#include "Narrative/NarrativeWidgetBase.h"
#include "GuidePages/GuideWidgetBase.h"
#include "ToroRuntimeSettings.h"

AToroWidgetManager::AToroWidgetManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

EToroValidPins AToroWidgetManager::GetToroWidgetManager(AToroWidgetManager*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroWidgetManager>& Class, const int32 PlayerIndex)
{
	AToroWidgetManager* Obj = Cast<AToroWidgetManager>(UGameplayStatics::GetPlayerState(WorldContextObject, PlayerIndex));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

UToroWidget* AToroWidgetManager::FindOrAddWidget(const TSubclassOf<UToroWidget> Class)
{
	if (!Class) return nullptr;
	UToroWidget* Obj = FindWidget(Class);
	if (Obj) return Obj;
	
	Obj = UToroWidget::CreateNew(PlayerController, Class);
	if (Obj) WidgetObjs.Add(Obj);
	return Obj;
}

// ReSharper disable once CppPassValueParameterByConstReference
UToroWidget* AToroWidgetManager::FindWidget(const TSubclassOf<UToroWidget> Class)
{
	if (!Class) return nullptr;
	WidgetObjs.Remove(nullptr);
	for (const TObjectPtr<UToroWidget>& Obj : WidgetObjs)
	{
		if (Obj && Obj->IsA(Class)) return Obj;
	}

	return nullptr;
}

void AToroWidgetManager::QueueSmallNotice(const FSimpleMessageData& NoticeData, const bool bResetQueue)
{
	if (UMessageWidgetBase* Widget = FindWidget<UMessageWidgetBase>())
	{
		Widget->QueueSmallNotice(NoticeData, bResetQueue);
	}
}

void AToroWidgetManager::QueueLargeNotice(const FSimpleMessageData& NoticeData, const bool bResetQueue)
{
	if (UMessageWidgetBase* Widget = FindWidget<UMessageWidgetBase>())
	{
		Widget->QueueLargeNotice(NoticeData, bResetQueue);
	}
}

void AToroWidgetManager::QueueSubtitles(const TArray<FSimpleSubtitleData>& Subtitles, const bool bOverride)
{
	if (UMessageWidgetBase* Widget = FindWidget<UMessageWidgetBase>())
	{
		Widget->QueueSubtitles(Subtitles, bOverride);
	}
}

void AToroWidgetManager::QueueSubtitle(const FSimpleSubtitleData& SubtitleData, const bool bOverride)
{
	if (UMessageWidgetBase* Widget = FindWidget<UMessageWidgetBase>())
	{
		Widget->QueueSubtitle(SubtitleData, bOverride);
	}
}

void AToroWidgetManager::AddControlEntry(const UInputAction* InAction)
{
	if (UMessageWidgetBase* Widget = FindWidget<UMessageWidgetBase>())
	{
		Widget->AppendControlEntry(InAction);
	}
}

void AToroWidgetManager::RemoveControlEntry(const UInputAction* InAction)
{
	if (UMessageWidgetBase* Widget = FindWidget<UMessageWidgetBase>())
	{
		Widget->RemoveControlEntry(InAction);
	}
}

void AToroWidgetManager::QueueGuidePage(const FGameplayTag InPageID)
{
	if (UGuideWidgetBase* Widget = FindWidget<UGuideWidgetBase>())
	{
		Widget->QueuePage(InPageID);
	}
}

void AToroWidgetManager::QueueGuidePages(const TArray<FGameplayTag>& InPageIDs)
{
	if (UGuideWidgetBase* Widget = FindWidget<UGuideWidgetBase>())
	{
		Widget->QueuePages(InPageIDs);
	}
}

void AToroWidgetManager::ToggleNarrativeQuests()
{
	if (UNarrativeWidgetBase* Widget = FindWidget<UNarrativeWidgetBase>())
	{
		// TODO: Narrative widget
		// Widget->SetQuestsHidden(Widget->AreQuestsHidden());
	}
}

void AToroWidgetManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<AToroPlayerController>(GetPlayerController());
	const TSet<TSoftClassPtr<UToroWidget>>& Widgets = UToroRuntimeSettings::Get()->DefaultWidgets;
	for (const TSoftClassPtr<UToroWidget>& Widget : Widgets)
	{
		FindOrAddWidget(Widget.LoadSynchronous());
	}
}
