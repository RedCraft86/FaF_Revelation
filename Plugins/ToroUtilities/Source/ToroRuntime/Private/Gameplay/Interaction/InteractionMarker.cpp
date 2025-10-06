// Copyright (C) RedCraft86. All Rights Reserved.

#include "Interaction/InteractionMarker.h"
#include "Kismet/GameplayStatics.h"
#if WITH_EDITOR
#include "Subsystems/UnrealEditorSubsystem.h"
#endif

UInteractionMarker::UInteractionMarker(): MaxDistance(250.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 0.1f;
	
	SetHiddenInGame(false);
	bIsScreenSizeScaled = true;
	ScreenSize = 0.0035f;
	OpacityMaskRefVal = 0.4f;

#if WITH_EDITOR
	bTickInEditor = true;
	const ConstructorHelpers::FObjectFinder<UTexture2D> SpriteFinder(
		TEXT("/ToroUtilities/Assets/T_Interactable.T_Interactable"));
	if (SpriteFinder.Succeeded())
	{
		UBillboardComponent::SetSprite(SpriteFinder.Object);
	}
#endif
}

FVector UInteractionMarker::GetCameraPos() const
{
	FVector Position = GetComponentLocation();
#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		if (!GEditor) return Position;
		if (UUnrealEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>())
		{
			FRotator Rotation;
			Subsystem->GetLevelViewportCameraInfo(Position, Rotation);
		}
	}
	else
#endif
		if (CamManager)
		{
			Position = CamManager->GetCameraLocation();
		}

	return Position;
}

void UInteractionMarker::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	});
}

void UInteractionMarker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->IsHidden()) return;
	SetVisibility(FVector::Dist(GetComponentLocation(), GetCameraPos()) <= MaxDistance);
}

