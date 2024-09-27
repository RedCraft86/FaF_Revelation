// Copyright (C) RedCraft86. All Rights Reserved.

#include "CCTVMonitor.h"
#include "FRPlayer.h"
#include "FRGameMode.h"
#include "CCTVCamera.h"
#include "CCTVScreenWidget.h"
#include "FRPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/NarrativeWidget.h"
#include "GameFramework/InputSettings.h"

ACCTVMonitor::ACCTVMonitor() : bZoomedIn(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	MonitorAudio = CreateDefaultSubobject<UAudioComponent>("MonitorAudio");
	MonitorAudio->SetupAttachment(SceneRoot);
	MonitorAudio->bAutoActivate = false;
	MonitorAudio->bCanPlayMultipleInstances = true;
	MonitorAudio->bOverrideAttenuation = true;
	MonitorAudio->AttenuationOverrides.bAttenuate = true;
	MonitorAudio->AttenuationOverrides.AttenuationShapeExtents = {100.0f, 0.0f, 0.0f};
	MonitorAudio->AttenuationOverrides.FalloffDistance = 600.0f;
	MonitorAudio->AttenuationOverrides.bEnableOcclusion = true;
	MonitorAudio->AttenuationOverrides.OcclusionVolumeAttenuation = 0.1f;
	
	MonitorMesh = CreateDefaultSubobject<UStaticMeshComponent>("MonitorMesh");
	MonitorMesh->SetupAttachment(SceneRoot);

	MonitorWidget = CreateDefaultSubobject<UWidgetComponent>("MonitorWidget");
	MonitorWidget->SetupAttachment(MonitorMesh);

	MonitorCamera = CreateDefaultSubobject<UCameraComponent>("MonitorCamera");
	MonitorCamera->SetupAttachment(SceneRoot);
	MonitorCamera->SetFieldOfView(80.0f);

	ChangeCameraStatic.InterpSpeed = 10.0f;
}

TArray<FName> ACCTVMonitor::GetCameraOptions() const
{
	TArray<FName> Keys;
	Cameras.GenerateKeyArray(Keys);
	return Keys;
}

void ACCTVMonitor::ChangeCamera(const FName InKey)
{
	if (InKey.IsNone() || !Cameras.Contains(InKey)
		|| ActiveCamera.Key == InKey) return;

	if (ActiveCamera.Value) ActiveCamera.Value->SetEnabled(false);
	
	ChangeCameraStatic.SnapToTarget();
	MonitorMesh->SetCustomPrimitiveDataFloat(0, IsEnabled() ? ChangeCameraStatic.CurrentValue : 1.0f);

	ActiveCamera = {InKey, Cameras[InKey]};
	if (ActiveCamera.Value) ActiveCamera.Value->SetEnabled(IsEnabled());
}

void ACCTVMonitor::PlayMonitorAudio(USoundBase* Sound, const float Volume) const
{
	MonitorAudio->SetSound(Sound);
	MonitorAudio->VolumeMultiplier = Volume;
	MonitorAudio->Play();
}

bool ACCTVMonitor::GetInteractionInfo_Implementation(FInteractionInfo& Info)
{
	Info = {};
	Info.Label = INVTEXT("Cameras");
	return !bZoomedIn && IsEnabled();
}

void ACCTVMonitor::OnBeginInteract_Implementation(AFRPlayerBase* Player, const FHitResult& HitResult)
{
	if (bZoomedIn)
		return;

	if (!CCTVMapWidget) CCTVMapWidget = CreateWidget<UCCTVMapWidget>(GetWorld(), MapWidget);
	if (CCTVMapWidget) CCTVMapWidget->AddToViewport(99);
	
	bZoomedIn = true;
	PlayerChar = Player;
	Player->SetWorldDevice(this);
	Player->GetGameMode()->SetGameInputMode(EGameInputMode::GameAndUI, true);
	Player->GetGameMode()->AddControlEntry({*(GetName() + TEXT("_Exit")), INVTEXT("Exit"), {EKeys::SpaceBar}});
	Player->GetGameMode()->AddControlEntry({*(GetName() + TEXT("_WASD")), INVTEXT("Turn"), {EKeys::W, EKeys::A, EKeys::S, EKeys::D}});
	Player->GetGameMode()->GetWidget<UNarrativeWidgetBase>()->SetQuestsHidden(true);
	ZoomIntoMonitor(Player->GetPlayerController());
	
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, [this, Player]()
	{
		EnableInput(Player->GetPlayerController());
	}, 0.1f , false);
}

void ACCTVMonitor::InputBinding_Turn(const FInputActionValue& InValue)
{
	if (ActiveCamera.Value) ActiveCamera.Value->TurnCamera(InValue.Get<FVector2D>());
}

void ACCTVMonitor::InputBinding_Exit(const FInputActionValue& InValue)
{
	if (!PlayerChar) return;
	if (CCTVMapWidget) CCTVMapWidget->RemoveWidget();
	
	PlayerChar->SetWorldDevice(nullptr);
	PlayerChar->GetGameMode()->SetGameInputMode(EGameInputMode::GameOnly);
	PlayerChar->GetGameMode()->GetWidget<UNarrativeWidgetBase>()->SetQuestsHidden(false);
	PlayerChar->GetGameMode()->RemoveControlEntry(*(GetName() + TEXT("_Exit")));
	PlayerChar->GetGameMode()->RemoveControlEntry(*(GetName() + TEXT("_WASD")));
	ZoomOutOfMonitor(PlayerChar->GetPlayerController(), PlayerChar);
	DisableInput(PlayerChar->GetPlayerController());
	PlayerChar = nullptr;
	
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, [this]()
	{
		bZoomedIn = false;
	}, 0.1f , false);
}

void ACCTVMonitor::UpdateCameraStatic()
{
	ChangeCameraStatic.TargetValue = ActiveCamera.Value && ActiveCamera.Value->IsFunctional() ? 0.0f : 1.0f;
}

void ACCTVMonitor::BeginPlay()
{
	Super::BeginPlay();
	for (const TArray<FName> CamKeys = GetCameraOptions(); const FName& Key : CamKeys)
	{
		if (ACCTVCamera* Cam = Cameras[Key])
		{
			Cam->SetMonitor(this);
		}
		else Cameras.Remove(Key);
	}

	if (!Cameras.Contains(DefaultCamera) && Cameras.Num() > 0)
	{
		DefaultCamera = GetCameraOptions()[0];
	}

	if (UCCTVScreenWidget* Widget = Cast<UCCTVScreenWidget>(MonitorWidget->GetWidget()))
	{
		Widget->Monitor = this;
	}

	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		ChangeCamera(DefaultCamera);	
	});

	CreateInputComponent(UInputSettings::GetDefaultInputComponentClass());
	if (UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		InputComp->BindAction(TurnInput, ETriggerEvent::Triggered, this, &ACCTVMonitor::InputBinding_Turn);
		InputComp->BindAction(ExitInput, ETriggerEvent::Started, this, &ACCTVMonitor::InputBinding_Exit);
	}
}

void ACCTVMonitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!ChangeCameraStatic.IsComplete())
	{
		ChangeCameraStatic.Tick(DeltaTime);
		MonitorMesh->SetCustomPrimitiveDataFloat(0, IsEnabled() ? ChangeCameraStatic.CurrentValue : 1.0f);
	}
}

void ACCTVMonitor::OnEnableStateChanged(const bool bIsEnabled)
{
	if (ActiveCamera.Value) ActiveCamera.Value->SetEnabled(bIsEnabled);
	SetActorEnableCollision(bIsEnabled);
	SetActorTickEnabled(bIsEnabled);
	UpdateCameraStatic();
}

