// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryPreview.h"
#include "Inventory/InventoryComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "PrimitiveData.h"

AInventoryPreview::AInventoryPreview() : TurnSpeedRate(5.0f, 2.0f), ZoomSpeedRate(5.0f, 0.1f)
	, ZoomRange(FVector2D::UnitVector), RotationValue(FRotator::ZeroRotator), ZoomValue({1.0f})
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
#if WITH_EDITOR
	SceneRoot->bVisualizeComponent = true;
#endif

	PreviewRoot = CreateDefaultSubobject<USpringArmComponent>("PreviewRoot");
	PreviewRoot->SetupAttachment(SceneRoot);
	PreviewRoot->bEnableCameraRotationLag = true;
	PreviewRoot->bDoCollisionTest = false;
	PreviewRoot->TargetArmLength = 0.0f;

	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>("PreviewMesh");
	PreviewMesh->SetupAttachment(PreviewRoot, USpringArmComponent::SocketName);
	PreviewMesh->SetLightingChannels(false, true, false);

	PreviewLight = CreateDefaultSubobject<USpotLightComponent>("PreviewLight");
	PreviewLight->SetRelativeLocation(FVector(-150.0f, 100.0f, 80.0f));
	PreviewLight->SetRelativeRotation(FRotator(-35.0f, 30.0f, 0.0f));
	PreviewLight->SetLightingChannels(false, true, false);
	PreviewLight->SetupAttachment(SceneRoot);

	PreviewCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("PreviewCapture");
	PreviewCapture->SetRelativeLocation(FVector(-150.0f, 0.0f, 0.0f));
	PreviewCapture->SetupAttachment(SceneRoot);

	PreviewCapture->FOVAngle = 60.0f;
	PreviewCapture->bCaptureEveryFrame = false;
	PreviewCapture->bAlwaysPersistRenderingState = true;
	PreviewCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	PreviewCapture->ShowOnlyActors = {this};
	PreviewCapture->ShowOnlyComponent(PreviewMesh);

	PreviewCapture->ShowFlags.SetAntiAliasing(true);
	PreviewCapture->ShowFlags.SetAtmosphere(false);
	PreviewCapture->ShowFlags.SetBSP(false);
	PreviewCapture->ShowFlags.SetCloud(false);
	PreviewCapture->ShowFlags.SetDecals(false);
	PreviewCapture->ShowFlags.SetFog(false);
	PreviewCapture->ShowFlags.SetLandscape(false);
	PreviewCapture->ShowFlags.SetParticles(false);
	PreviewCapture->ShowFlags.SetSkeletalMeshes(false);
	PreviewCapture->ShowFlags.SetStaticMeshes(true);
	PreviewCapture->ShowFlags.SetTranslucency(true);
	PreviewCapture->ShowFlags.SetDeferredLighting(true);
	PreviewCapture->ShowFlags.SetInstancedFoliage(false);
	PreviewCapture->ShowFlags.SetInstancedGrass(false);
	PreviewCapture->ShowFlags.SetInstancedStaticMeshes(false);
	PreviewCapture->ShowFlags.SetNaniteMeshes(true);
	PreviewCapture->ShowFlags.SetPaper2DSprites(false);
	PreviewCapture->ShowFlags.SetTextRender(false);
	PreviewCapture->ShowFlags.SetTemporalAA(true);
	PreviewCapture->ShowFlags.SetBloom(true);
	PreviewCapture->ShowFlags.SetEyeAdaptation(true);
	PreviewCapture->ShowFlags.SetLocalExposure(true);
	PreviewCapture->ShowFlags.SetMotionBlur(false);
	PreviewCapture->ShowFlags.SetToneCurve(true);
	PreviewCapture->ShowFlags.SetSkyLighting(true);
	PreviewCapture->ShowFlags.SetAmbientOcclusion(false);
	PreviewCapture->ShowFlags.SetDynamicShadows(true);
	PreviewCapture->ShowFlags.SetAmbientCubemap(true);
	PreviewCapture->ShowFlags.SetDistanceFieldAO(true);
	PreviewCapture->ShowFlags.SetLightFunctions(false);
	PreviewCapture->ShowFlags.SetLightShafts(false);
	PreviewCapture->ShowFlags.SetReflectionEnvironment(true);
	PreviewCapture->ShowFlags.SetScreenSpaceReflections(true);
	PreviewCapture->ShowFlags.SetTexturedLightProfiles(false);
	PreviewCapture->ShowFlags.SetVolumetricFog(false);
	PreviewCapture->ShowFlags.SetGame(true);
	PreviewCapture->ShowFlags.SetLighting(true);
	PreviewCapture->ShowFlags.SetPathTracing(false);
	PreviewCapture->ShowFlags.SetPostProcessing(true);
}

bool AInventoryPreview::SetItem(const FGuid& InSlotID)
{
	if (InSlotID.IsValid() && Inventory->IsValidSlot(InSlotID))
	{
		SlotID = InSlotID;
		const FInvSlotData& SlotData = Inventory->GetConstInventory().FindRef(InSlotID);
		const UInventoryItemData* ItemData = SlotData.Item.LoadSynchronous();
		const FTransformMeshData& MeshData = ItemData->GetMeshData(SlotData.Metadata);
		
		MeshData.ToStaticMeshComponent(PreviewMesh);
		PreviewMesh->SetRelativeTransform(MeshData.Transform);
		PreviewMesh->SetHiddenInGame(false);

		ZoomRange = ItemData->PreviewZoom;
		ZoomValue.Target = (ZoomRange.X + ZoomRange.Y) * 0.5f;
		PreviewRoot->SetRelativeScale3D(FVector(ZoomValue.Target));
		PreviewRoot->SetRelativeRotation(FRotator::ZeroRotator);

		return true;
	}

	PreviewMesh->SetHiddenInGame(true);
	return false;
}

void AInventoryPreview::Initialize()
{
	ZoomValue.InterpSpeed = ZoomSpeedRate.X;
	PreviewCapture->bCaptureEveryFrame = true;
	EnableInput(GetWorld()->GetFirstPlayerController());
	SetActorTickEnabled(true);
}

void AInventoryPreview::Deinitialize()
{
	PreviewCapture->bCaptureEveryFrame = false;
	ZoomRange = FVector2D::UnitVector;
	RotationValue = FRotator::ZeroRotator;
	PreviewRoot->SetRelativeRotation(FRotator::ZeroRotator);
	PreviewMesh->SetRelativeTransform(FTransform::Identity);
	DisableInput(GetWorld()->GetFirstPlayerController());
	SetActorTickEnabled(false);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AInventoryPreview::InputBinding_Turn(const FInputActionValue& InValue)
{
	const FVector2D Value = InValue.Get<FVector2D>();
	if (PreviewCapture->bCaptureEveryFrame && !FMath::IsNearlyZero(Value.Size()))
	{
		PreviewRoot->AddWorldRotation(FRotator(Value.Y, Value.X, 0.0f) * TurnSpeedRate.Y);
	}
}

void AInventoryPreview::InputBinding_Zoom(const FInputActionValue& InValue)
{
	const float Value = InValue.Get<float>();
	if (PreviewCapture->bCaptureEveryFrame && !FMath::IsNearlyZero(Value))
	{
		ZoomValue.Target = FMath::Clamp(ZoomValue.Target + Value * ZoomSpeedRate.Y, ZoomRange.X, ZoomRange.Y);
	}
}

void AInventoryPreview::BeginPlay()
{
	Super::BeginPlay();
	Super::BeginPlay();
	PreviewRoot->SetTickableWhenPaused(true);
	PreviewRoot->CameraRotationLagSpeed = TurnSpeedRate.X;
#if !WITH_EDITOR
	PreviewMesh->SetVisibleInSceneCaptureOnly(true);
#endif
	PreviewCapture->SetTickableWhenPaused(true);
	SetActorEnableCollision(false);
	Deinitialize();

	CreateInputComponent(UInputSettings::GetDefaultInputComponentClass());
	if (UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		InputComp->BindAction(TurnInput, ETriggerEvent::Triggered, this, &AInventoryPreview::InputBinding_Turn);
		InputComp->BindAction(ZoomInput, ETriggerEvent::Triggered, this, &AInventoryPreview::InputBinding_Zoom);
	}
}

void AInventoryPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SlotID.IsValid() && !ZoomValue.IsComplete())
	{
		ZoomValue.Tick(DeltaTime);
		PreviewRoot->SetRelativeScale3D(FVector(ZoomValue.Current));
	}
}

void AInventoryPreview::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	PreviewLight->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(
		PreviewLight->GetComponentLocation(), PreviewRoot->GetComponentLocation()));
}
