// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameMode.h"
#include "Framework/ToroWidgetManager.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "MusicSystem/MusicManager.h"

AToroGameMode::AToroGameMode()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	GameStateClass = AMusicManager::StaticClass();
	PlayerControllerClass = AToroPlayerController::StaticClass();
	PlayerStateClass = AToroWidgetManager::StaticClass();
	DefaultPawnClass = AToroPlayerCharacter::StaticClass();
}

EToroValidPins AToroGameMode::GetToroGameMode(AToroGameMode*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroGameMode>& Class)
{
	AToroGameMode* Obj = Cast<AToroGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

void AToroGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = UToroGameInstance::Get<UToroGameInstance>(this);
	if (GameInstance) GameInstance->OnWorldBeginPlay(GetWorld());
}

void AToroGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GameInstance) GameInstance->OnWorldTick(DeltaSeconds);
}
