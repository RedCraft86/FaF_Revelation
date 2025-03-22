// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Framework/ToroGameMode.h"

UE_DEFINE_GAMEPLAY_TAG_CHILD(Player, Character)

AToroPlayerCharacter::AToroPlayerCharacter() : SlowTickInterval(0.1f)
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	PlayerLight = CreateDefaultSubobject<UPointLightComponent>("PlayerLight");
	PlayerLight->SetupAttachment(GetCapsuleComponent());

	EquipmentRoot = CreateDefaultSubobject<USceneComponent>("EquipmentRoot");
	EquipmentRoot->SetupAttachment(GetCapsuleComponent());
#if WITH_EDITOR
	EquipmentRoot->bVisualizeComponent = true;
#endif

	CharacterID = Tag_Character_Player;
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

EToroValidPins AToroPlayerCharacter::GetToroPlayerCharacter(AToroPlayerCharacter*& OutObject,
	const UObject* ContextObject, const TSubclassOf<AToroPlayerCharacter>& Class, const int32 PlayerIndex)
{
	AToroPlayerCharacter* Obj = Get<AToroPlayerCharacter>(ContextObject, PlayerIndex);
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

void AToroPlayerCharacter::AddLockFlag(const FPlayerLockFlag& InFlag)
{
	if (InFlag.IsValidFlag()) LockFlags.Add(InFlag);
}

void AToroPlayerCharacter::ClearLockFlag(const FPlayerLockFlag& InFlag)
{
	if (InFlag.IsValidFlag()) LockFlags.Remove(InFlag);
}

bool AToroPlayerCharacter::HasLockFlag(const FPlayerLockFlag& InFlag) const
{
	return InFlag.IsValidFlag() && LockFlags.Contains(InFlag);
}

void AToroPlayerCharacter::SetLightSettings(const FPointLightProperties& InSettings)
{
	LightSettings = InSettings;
	ULightingDataLibrary::SetPointLightProperties(PlayerLight, LightSettings);
}

void AToroPlayerCharacter::EnterCinematic(AActor* CinematicActor)
{
	if (PlayerController) PlayerController->EnterCinematic(CinematicActor);
}

void AToroPlayerCharacter::ExitCinematic()
{
	if (PlayerController) PlayerController->ExitCinematic();
}

void AToroPlayerCharacter::FadeToBlack(const float InTime, const bool bAudio) const
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

void AToroPlayerCharacter::FadeFromBlack(const float InTime, const bool bAudio) const
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

void AToroPlayerCharacter::ClearFade() const
{
	if (PlayerController && PlayerController->PlayerCameraManager)
	{
		PlayerController->PlayerCameraManager->StopCameraFade();
	}
}

void AToroPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameMode = AToroGameMode::Get(this);
	PlayerController = AToroPlayerController::Get(this);
	GameInstance = UToroGameInstance::Get(this);
	GetWorldTimerManager().SetTimer(SlowTickTimer, this,
		&AToroPlayerCharacter::SlowTick, SlowTickInterval, true);
}

void AToroPlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	DeduplicateLockFlags(LockFlags);
	ULightingDataLibrary::SetPointLightProperties(PlayerLight, LightSettings);
}
