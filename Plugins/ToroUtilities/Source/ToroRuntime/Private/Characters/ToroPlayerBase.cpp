// Copyright (C) RedCraft86. All Rights Reserved.

#include "Characters/ToroPlayerBase.h"
#include "Components/CapsuleComponent.h"
#include "Framework/ToroPlayerController.h"

AToroPlayerBase::AToroPlayerBase()
	: SlowTickInterval(0.1f), ControlFlags(DEFAULT_PLAYER_CONTROL_FLAGS)
	, LockFlags({Tag_LockStartup.GetTag().GetTagName()})
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	FootstepAudio = CreateDefaultSubobject<UAudioComponent>("FootstepAudio");
	FootstepAudio->SetRelativeLocation(FVector(0.0f, 0.0f, -60.0f));
	FootstepAudio->SetupAttachment(GetCapsuleComponent());

	PlayerLight = CreateDefaultSubobject<UPointLightComponent>("PlayerLight");
	PlayerLight->SetupAttachment(GetCapsuleComponent());
	
	EquipmentRoot = CreateDefaultSubobject<USceneComponent>("EquipmentRoot");
	EquipmentRoot->SetupAttachment(GetCapsuleComponent());
#if WITH_EDITOR
	EquipmentRoot->bVisualizeComponent = true;
#endif
	
	CharacterID = Tag_Player;
	LightSettings.Intensity = 0.025f;
	LightSettings.AttenuationRadius = 500.0f;
	LightSettings.bUseTemperature = true;
	LightSettings.Temperature = 12000.0f;
	LightSettings.bLightCastShadows = false;
	LightSettings.bUseInverseSquaredFalloff = true;
	LightSettings.LightFalloffExponent = 1.0f;
	LightSettings.SpecularScale = 0.0f;
	ULightingDataLibrary::SetPointLightProperties(PlayerLight, LightSettings);
}

EToroValidPins AToroPlayerBase::GetToroPlayerCharacter(AToroPlayerBase*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroPlayerBase>& Class, const int32 PlayerIndex)
{
	AToroPlayerBase* Obj = Cast<AToroPlayerBase>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

void AToroPlayerBase::TeleportPlayer(const FVector& InLocation, const FRotator& InRotation)
{
	SetActorLocation(InLocation);
	Controller->SetControlRotation(InRotation);
}

void AToroPlayerBase::OverrideControlFlags(const int32 InFlags)
{
	for (const EPlayerControlFlags Enum : TEnumRange<EPlayerControlFlags>())
	{
		if (InFlags & Enum)
		{
			SetControlFlag(Enum);
		}
		else
		{
			UnsetControlFlag(Enum);
		}
	}
}

void AToroPlayerBase::SetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag == PCF_None) return;
	if (!HasControlFlag(InFlag))
	{
		ControlFlags |= InFlag;
	}
}

void AToroPlayerBase::UnsetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag == PCF_None) return;
	if (HasControlFlag(InFlag))
	{
		ControlFlags &= ~InFlag;
	}
}

bool AToroPlayerBase::HasControlFlag(const EPlayerControlFlags InFlag) const
{
	if (InFlag == PCF_None) return false;
	return ControlFlags & InFlag;
}

void AToroPlayerBase::SetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag == PSF_None) return;
	StateFlags |= InFlag;
}

void AToroPlayerBase::UnsetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag == PSF_None) return;
	StateFlags &= ~InFlag;
}

bool AToroPlayerBase::HasStateFlag(const EPlayerStateFlags InFlag) const
{
	if (InFlag == PSF_None) return false;
	return StateFlags & InFlag;
}

void AToroPlayerBase::AddLockFlag(const FPlayerLockFlag& InFlag)
{
	LockFlags.Remove(NAME_None);
	if (InFlag.IsValidFlag()) LockFlags.Add(*InFlag);
}

void AToroPlayerBase::ClearLockFlag(const FPlayerLockFlag& InFlag)
{
	LockFlags.Remove(NAME_None);
	if (InFlag.IsValidFlag()) LockFlags.Remove(*InFlag);
}

bool AToroPlayerBase::HasLockFlag(const FPlayerLockFlag& InFlag) const
{
	return InFlag.IsValidFlag() && LockFlags.Contains(*InFlag);
}

void AToroPlayerBase::ExitCinematic()
{
	if (CinematicActor)
	{
		CinematicActor = nullptr;
		ClearLockFlag(Tag_LockCinematic.GetTag());
		PlayerController->SetCinematicMode(false, false,
			false, true, true);
	}
}

void AToroPlayerBase::EnterCinematic(AActor* InActor)
{
	if (!CinematicActor)
	{
		CinematicActor = InActor;
		AddLockFlag(Tag_LockCinematic.GetTag());
		PlayerController->SetCinematicMode(true, true,
			false, true, true);
	}
}

void AToroPlayerBase::ClearEnemyStack()
{
	if (!EnemyStack.IsEmpty())
	{
		EnemyStack.Empty();
		OnEnemyStackChanged();
	}
}

void AToroPlayerBase::AddEnemy(AToroNPCBase* InEnemy)
{
	if (!EnemyStack.Contains(InEnemy))
	{
		EnemyStack.Add(InEnemy);
		EnemyStack.Remove(nullptr);
		OnEnemyStackChanged();
	}
}

void AToroPlayerBase::RemoveEnemy(AToroNPCBase* InEnemy)
{
	if (EnemyStack.Contains(InEnemy))
	{
		EnemyStack.Remove(InEnemy);
		EnemyStack.Remove(nullptr);
		OnEnemyStackChanged();
	}
}

void AToroPlayerBase::UpdateEnemy(const AToroNPCBase* InEnemy)
{
	if (EnemyStack.Contains(InEnemy))
	{
		EnemyStack.Remove(nullptr);
		OnEnemyStackChanged();
	}
}

void AToroPlayerBase::SetLightSettings(const FPointLightProperties& InSettings)
{
	LightSettings = InSettings;
	ULightingDataLibrary::SetPointLightProperties(PlayerLight, LightSettings);
}

void AToroPlayerBase::FadeToBlack(const float InTime, const bool bAudio) const
{
	if (PlayerController && PlayerController->PlayerCameraManager)
	{
		if (FMath::IsNearlyZero(InTime))
		{
			PlayerController->PlayerCameraManager->SetManualCameraFade(1.0f, FLinearColor::Black, bAudio);
		}
		else
		{
			PlayerController->PlayerCameraManager->StartCameraFade(0.0f, 1.0f,
			   InTime, FLinearColor::Black, bAudio, true);
		}
	}
}

void AToroPlayerBase::FadeFromBlack(const float InTime, const bool bAudio) const
{
	if (PlayerController && PlayerController->PlayerCameraManager)
	{
		if (FMath::IsNearlyZero(InTime))
		{
			PlayerController->PlayerCameraManager->SetManualCameraFade(0.0f, FLinearColor::Black, bAudio);
		}
		else
		{
			PlayerController->PlayerCameraManager->StartCameraFade(1.0f, 0.0f,
			   InTime, FLinearColor::Black, bAudio, true);
		}
	}
}

void AToroPlayerBase::ClearFade() const
{
	if (PlayerController && PlayerController->PlayerCameraManager)
	{
		PlayerController->PlayerCameraManager->StopCameraFade();
	}
}

void AToroPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	GameMode = AToroGameMode::Get(this);
	PlayerController = AToroPlayerController::Get(this);
	GameInstance = UToroGameInstance::Get(this);

	GetWorldTimerManager().SetTimer(SlowTickTimer, this,
		&AToroPlayerBase::SlowTick, SlowTickInterval, true);
}

void AToroPlayerBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ULightingDataLibrary::SetPointLightProperties(PlayerLight, LightSettings);
}
