// Copyright (C) RedCraft86. All Rights Reserved.

#include "GenericDoor.h"
#include "Components/AudioComponent.h"
#include "Libraries/ToroMathLibrary.h"
#include "Libraries/ToroGeneralUtils.h"
#include "Inventory/InventoryManager.h"

AGenericDoor::AGenericDoor(): bStartOpened(false), OpenDirection(EOpenDirection::Dynamic), OpenAngle(100.0f)
	, LockedLabel(INVTEXT("Requires {Key}")), bOpened(false), DoorAngle(0.0f), TimeRange(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	DoorPivot = CreateDefaultSubobject<USceneComponent>("DoorPivot");
	DoorPivot->SetupAttachment(SceneRoot);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetupAttachment(DoorPivot);

	OpenAudio = CreateDefaultSubobject<UAudioComponent>("OpenAudio");
	OpenAudio->bAutoActivate = false;
	OpenAudio->bOverrideAttenuation = true;
	OpenAudio->bCanPlayMultipleInstances = true;
	OpenAudio->AttenuationOverrides.FalloffDistance = 400.0f;
	OpenAudio->AttenuationOverrides.AttenuationShapeExtents.X = 200.0f;
	OpenAudio->SetupAttachment(MeshComponent);

	CloseAudio = CreateDefaultSubobject<UAudioComponent>("CloseAudio");
	CloseAudio->bAutoActivate = false;
	CloseAudio->bOverrideAttenuation = true;
	CloseAudio->bCanPlayMultipleInstances = true;
	CloseAudio->AttenuationOverrides.FalloffDistance = 500.0f;
	CloseAudio->AttenuationOverrides.AttenuationShapeExtents.X = 250.0f;
	CloseAudio->SetupAttachment(MeshComponent);

	Animation.GetRichCurve()->SetKeyTangentMode(Animation.GetRichCurve()
		->UpdateOrAddKey(0.0f, 0.0f), RCTM_Auto);
	
	Animation.GetRichCurve()->SetKeyTangentMode(Animation.GetRichCurve()
		->UpdateOrAddKey(0.5f, 0.0f), RCTM_Auto);

	TimeLerp = {0.0f, 1.0f};
	TimeLerp.bConstant = true;
}

void AGenericDoor::SetOpened(const bool bInOpened, const bool bImmediate)
{
	if (bOpened != bInOpened)
	{
		bOpened = bInOpened;
		if (bOpened)
		{
			DoorAngle = OpenAngle;
			if (OpenDirection == EOpenDirection::Flipped || (OpenDirection == EOpenDirection::Dynamic
					&& !UToroGeneralUtils::IsActorInFront(this, Interactor.Get())))
			{
				DoorAngle *= -1.0f;
			}
			if (!bImmediate)
			{
				OpenAudio->Play();
			}
		}
		else if (!bImmediate)
		{
			CloseAudio->Play();
		}

		TimeLerp.Target = bOpened ? TimeRange.X : TimeRange.Y;
		if (bImmediate)
		{
			TimeLerp.SnapToTarget();
			UpdateDoorAngle();
		}
	}
}

void AGenericDoor::UpdateDoorAngle() const
{
	DoorPivot->SetRelativeRotation(FRotator{0.0f,
		FMath::Lerp(0.0f, DoorAngle, Animation.GetValue(TimeLerp.Current)),
		0.0f
	});
}

bool AGenericDoor::GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info)
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
	return bReturn && TimeLerp.IsComplete();
}

void AGenericDoor::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit)
{
	Interactor = Player;
	if (!IsLocked())
	{
		SetOpened(!bOpened);
	}
	else if (Inventory && Inventory->HasItem(KeyAsset))
	{
		if (!KeyAsset->bReusableKey)
		{
			Inventory->RemoveItem(KeyAsset);
		}
		KeyAsset = nullptr;
	}
}

void AGenericDoor::OnPawnInteract_Implementation(APawn* Pawn, const FHitResult& Hit)
{
	Interactor = Pawn;
	if (!IsLocked())
	{
		SetOpened(true);
	}
}

void AGenericDoor::BeginPlay()
{
	Super::BeginPlay();
	Animation.GetTimeRange(TimeRange.X, TimeRange.Y);

	if (bStartOpened)
	{
		SetOpened(true, true);
	}

	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		Inventory = UInventoryManager::Get(this);
	});
}

void AGenericDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!TimeLerp.IsComplete())
	{
		TimeLerp.Tick(DeltaTime);
		UpdateDoorAngle();
	}
}

void AGenericDoor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	DoorMesh.FillMaterials();
	DoorMesh.ToStaticMeshComponent(MeshComponent);
	MeshComponent->SetRelativeTransform(DoorMesh.Transform);

#if WITH_EDITOR
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