// Copyright (C) RedCraft86. All Rights Reserved.

#include "DoorBase.h"
#include "Components/AudioComponent.h"
#include "Inventory/InventoryManager.h"
#include "NativeWidgets/NoticeWidget.h"
#if WITH_EDITOR
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#endif

ADoorBase::ADoorBase(): bStartOpened(false), PlayRate(1.0f), bOpened(false)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DoorRange = CreateDefaultSubobject<UBoxComponent>("DoorRange");
	DoorRange->SetCollisionResponseToAllChannels(ECR_Ignore);
	DoorRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DoorRange->SetupAttachment(GetRootComponent());
	DoorRange->SetBoxExtent(FVector(125.0f, 125.0f, 75.0f));

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
	CloseAudio->SetupAttachment(GetRootComponent());

	CurvePlayer = CreateDefaultSubobject<UCurvePlayerFloat>("CurvePlayer");

	Marker->SetVisibility(false);
	Marker->SetHiddenInGame(true);

	Animation.AddOrUpdatePoint(0.0f, 0.0f);
	Animation.AddOrUpdatePoint(0.5f, 1.0f);
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

void ADoorBase::OpenStateChanged_Implementation(const bool bState, const bool bImmediate)
{
	FVector2D TimeRange;
	Animation.GetTimeRange(TimeRange.X, TimeRange.Y);

	CurvePlayer->PlayRate = PlayRate;
	if (bImmediate)
	{
		CurvePlayer->SetPlaybackTime(bState ? TimeRange.Y : TimeRange.X);
	}
	else
	{
		bState ? CurvePlayer->Play(CurvePlayer->GetTime()) : CurvePlayer->Reverse();
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
	// If our last interactor is by a non-player entity, disallow interaction until they go out of range
	return bReturn && (!Interactor.IsValid() || CharacterTags::IsPlayer(Interactor.Get()));
}

void ADoorBase::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit)
{
	Interactor = Player;
	if (IsLocked() && Inventory && Inventory->UseKey(KeyAsset))
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
	CurvePlayer->PlayRate = PlayRate;
	CurvePlayer->SetCurve(Animation);
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		SetOpened(bStartOpened, true);
		Inventory = UInventoryManager::Get(this);
	});
}

void ADoorBase::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (OtherActor && OtherActor->IsA<AToroCharacter>())
	{
		OtherActor->Tags.Remove(CharacterTags::NearInteractable);
	}
}

void ADoorBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor && OtherActor->IsA<AToroCharacter>())
	{
		OtherActor->Tags.Add(CharacterTags::NearInteractable);
	}
}

void ADoorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITOR
	FVector Origin, Extent;
	GetActorBounds(true, Origin, Extent, false);
	DoorRange->SetWorldLocation(Origin);
	OpenAudio->SetWorldLocation(Origin);
	CloseAudio->SetWorldLocation(Origin);

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
