// Copyright (C) RedCraft86. All Rights Reserved.

#include "DoorBase.h"
#include "Components/AudioComponent.h"
#include "Inventory/InventoryManager.h"
#include "NativeWidgets/NoticeWidget.h"
#if WITH_EDITOR
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#endif

ADoorBase::ADoorBase(): bStartOpened(false), bOpened(false)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	OpenAudio = CreateDefaultSubobject<UAudioComponent>("OpenAudio");
	OpenAudio->bAutoActivate = false;
	OpenAudio->bOverrideAttenuation = true;
	OpenAudio->bCanPlayMultipleInstances = true;
	OpenAudio->AttenuationOverrides.FalloffDistance = 400.0f;
	OpenAudio->AttenuationOverrides.AttenuationShapeExtents.X = 200.0f;
	OpenAudio->SetupAttachment(GetRootComponent());

	CloseAudio = CreateDefaultSubobject<UAudioComponent>("CloseAudio");
	CloseAudio->bAutoActivate = false;
	CloseAudio->bOverrideAttenuation = true;
	CloseAudio->bCanPlayMultipleInstances = true;
	CloseAudio->AttenuationOverrides.FalloffDistance = 500.0f;
	CloseAudio->AttenuationOverrides.AttenuationShapeExtents.X = 250.0f;
	CloseAudio->SetupAttachment(OpenAudio);
}

void ADoorBase::SetOpened(const bool bInOpened, const bool bImmediate)
{
	if (bOpened != bInOpened)
	{
		bOpened = bInOpened;
		OpenStateChanged(bOpened, bImmediate);
		if (!bImmediate)
		{
			bOpened ? OpenAudio->Play() : CloseAudio->Play();
		}
	}
}

bool ADoorBase::GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info)
{
	const bool bReturn = Super::GetInteractInfo_Implementation(Hit, Info);
	if (KeyAsset)
	{
		Info.Label = FText::FormatNamed(LockedLabel, TEXT("Key"), KeyAsset->DisplayName);
	}
	else
	{
		Info.Label = bOpened ? INVTEXT("Close") : INVTEXT("Open");
	}
	return bReturn;
}

void ADoorBase::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit)
{
	Interactor = Player;
	if (Inventory && Inventory->UseKey(KeyAsset))
	{
		KeyAsset = nullptr;
		UNoticeWidget::QueueNotice(this, {
			INVTEXT("Door Unlocked")
		}, true);
	}
	if (!IsLocked())
	{
		SetOpened(!bOpened);
	}
}

void ADoorBase::OnPawnInteract_Implementation(APawn* Pawn, const FHitResult& Hit)
{
	Interactor = Pawn;
	if (!IsLocked())
	{
		SetOpened(true);
	}
}

void ADoorBase::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		SetOpened(bStartOpened, true);
		Inventory = IsLocked() ? UInventoryManager::Get(this) : nullptr;
	});
}

void ADoorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITOR
	FVector Origin, Extent;
	GetActorBounds(true, Origin, Extent, false);
	OpenAudio->SetWorldLocation(Origin);

	if (KeyAsset && KeyAsset->AssetType == EInvAssetType::Item
		&& KeyAsset->ItemType == EInvItemType::KeyItem)
	{
		FNotificationInfo Info(FText::Format(
			INVTEXT("\"{0}\" is not a valid Key Item"),
			FText::FromString(GetNameSafe(KeyAsset)))
		);
		Info.ExpireDuration = 1.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
		KeyAsset = nullptr;
	}
#endif
}
