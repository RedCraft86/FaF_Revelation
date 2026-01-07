// Copyright (C) RedCraft86. All Rights Reserved.

#include "DoorBase.h"
#include "Components/AudioComponent.h"
#include "Inventory/InventoryManager.h"
#include "NativeWidgets/NoticeWidget.h"
#include "NavAreas/NavArea_Default.h"
#if WITH_EDITOR
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#endif

ADoorBase::ADoorBase(): bStartOpened(false), PlayRate(1.0f), bSmartLink(true), bOpened(false)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DoorRange = CreateDefaultSubobject<UBoxComponent>("DoorRange");
	DoorRange->SetCollisionResponseToAllChannels(ECR_Ignore);
	DoorRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DoorRange->SetupAttachment(GetRootComponent());
	DoorRange->SetBoxExtent(FVector(125.0f, 125.0f, 75.0f));
	DoorRange->SetCanEverAffectNavigation(false);

	OpenAudio = CreateDefaultSubobject<UAudioComponent>("OpenAudio");
	OpenAudio->SetupAttachment(GetRootComponent());
	OpenAudio->bAutoActivate = false;
	OpenAudio->bOverrideAttenuation = true;
	OpenAudio->bCanPlayMultipleInstances = true;
	OpenAudio->AttenuationOverrides.FalloffDistance = 400.0f;
	OpenAudio->AttenuationOverrides.AttenuationShapeExtents.X = 200.0f;

	CloseAudio = CreateDefaultSubobject<UAudioComponent>("CloseAudio");
	CloseAudio->SetupAttachment(GetRootComponent());
	CloseAudio->bAutoActivate = false;
	CloseAudio->bOverrideAttenuation = true;
	CloseAudio->bCanPlayMultipleInstances = true;
	CloseAudio->AttenuationOverrides.FalloffDistance = 500.0f;
	CloseAudio->AttenuationOverrides.AttenuationShapeExtents.X = 250.0f;

	DoorLinkComponent = CreateDefaultSubobject<UChildActorComponent>("DoorLink");
	DoorLinkComponent->SetupAttachment(GetRootComponent());
	DoorLinkComponent->SetChildActorClass(ADoorLink::StaticClass());

	NavPoints.bUseSnapHeight = true;
	NavPoints.LeftProjectHeight = 1000.0f;
	NavPoints.Left = FVector(100.0f, 0.0f, 0.0f);
	NavPoints.Right = FVector(-100.0f, 0.0f, 0.0f);
	NavPoints.SetAreaClass(UNavArea_Default::StaticClass());

	if (ADoorLink* DoorLinkActor = Cast<ADoorLink>(DoorLinkComponent->GetChildActor()))
	{
		DoorLinkActor->bSmartLinkIsRelevant = bSmartLink;
#if WITH_EDITOR
		DoorLinkActor->PointLinks = {NavPoints};
#endif
	}

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

#if WITH_EDITORONLY_DATA
void ADoorBase::TryDetermineNavLinks()
{
	if (ADoorLink* DoorLinkActor = Cast<ADoorLink>(DoorLinkComponent->GetChildActor()))
	{
		DoorLinkActor->bSmartLinkIsRelevant = bSmartLink;

		FVector Origin, Extent;
		GetActorBounds(true, Origin, Extent, false);

		Origin.Z = GetActorLocation().Z;
		const float Distance = FMath::Max3(Extent.X, Extent.Y, 5.0) * NavPointOffsetMulti;
		bool bValue = Extent.X > Extent.Y; 
		if (bFlipXY) bValue = !bValue;
		const FVector WorldForward = bValue ? FVector::XAxisVector : FVector::YAxisVector;
		const FVector LocalForward = GetTransform().InverseTransformVectorNoScale(WorldForward);
		const FVector LocalOffset = GetTransform().InverseTransformPosition(Origin);

		NavPoints.Left = LocalOffset + (LocalForward * Distance);
		NavPoints.Right = LocalOffset + (LocalForward * -Distance);

		DoorLinkActor->PointLinks = {NavPoints};

		DoorLinkActor->CopyEndPointsFromSimpleLinkToSmartLink();

		Modify();
	}
}
#endif

void ADoorBase::OnEntityReachedDoor(AActor* Entity, const FVector& Dest)
{
	if (IsLocked())
	{
		ICharInterface::OnPathingRejected(Entity, EPathingRejectType::Door);
		UE_LOG(LogTemp, Warning, TEXT("Rejected: %s -> %s"), *GetNameSafe(Entity), *GetActorLabel())
	}
	else
	{
		IInteractionInterface::PawnInteract(this, Entity);
		ICharInterface::OnEntityInteraction(Entity, ECharInteractType::Door, this);
		UE_LOG(LogTemp, Warning, TEXT("Reached: %s -> %s"), *GetNameSafe(Entity), *GetActorLabel())
	}
	
	if (DoorLink)
	{
		DoorLink->ResumePathFollowing(Entity);
		// GetWorldTimerManager().ClearTimer(DoorLinkTimer);
		// GetWorldTimerManager().SetTimer(DoorLinkTimer, [this, Entity]()
		// {
		// 	DoorLink->ResumePathFollowing(Entity);
		// }, 0.5f, false);
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

void ADoorBase::OnPawnInteract_Implementation(AActor* Pawn)
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

	if (DoorLink = Cast<ADoorLink>(DoorLinkComponent->GetChildActor()); DoorLink)
	{
		DoorLink->PointLinks = {NavPoints};
		DoorLink->CopyEndPointsFromSimpleLinkToSmartLink();
		DoorLink->GetOnSmartLinkReached().AddDynamic(this, &ADoorBase::OnEntityReachedDoor);
	}

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
		if (OtherActor == Interactor)
		{
			Interactor = nullptr;
		}
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

	if (ADoorLink* DoorLinkActor = Cast<ADoorLink>(DoorLinkComponent->GetChildActor()))
	{
		DoorLinkActor->PointLinks = {NavPoints};
#if WITH_EDITOR
		DoorLinkActor->CopyEndPointsFromSimpleLinkToSmartLink();
#endif
	}
}
