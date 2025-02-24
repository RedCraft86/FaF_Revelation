// Copyright (C) RedCraft86. All Rights Reserved.

#include "Characters/ToroPlayerBase.h"
#include "Components/CapsuleComponent.h"
#include "Framework/ToroPlayerController.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_Player, "Characters.Player");

AToroPlayerBase::AToroPlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	FootstepAudio = CreateDefaultSubobject<UAudioComponent>("FootstepAudio");
	FootstepAudio->SetRelativeLocation(FVector(0.0f, 0.0f, -60.0f));
	FootstepAudio->SetupAttachment(GetCapsuleComponent());

	PlayerLight = CreateDefaultSubobject<UPointLightComponent>("PlayerLight");
	PlayerLight->SetupAttachment(GetCapsuleComponent());
	
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

AToroPlayerController* AToroPlayerBase::GetPlayerController() const
{
	return GetController<AToroPlayerController>();
}

void AToroPlayerBase::BeginPlay()
{
	Super::BeginPlay();
	GameMode = AToroGameMode::Get(this);
	GameInstance = UToroGameInstance::Get(this);
}

void AToroPlayerBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ULightingDataLibrary::SetPointLightProperties(PlayerLight, LightSettings);
}
