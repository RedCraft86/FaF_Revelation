// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameMode.h"
#include "Framework/ToroMusicManager.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroWidgetManager.h"
#include "Characters/ToroPlayerBase.h"

AToroGameMode::AToroGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	GameStateClass = AToroMusicManager::StaticClass();
	PlayerControllerClass = AToroPlayerController::StaticClass();
	PlayerStateClass = AToroWidgetManager::StaticClass();
	DefaultPawnClass = AToroPlayerBase::StaticClass();
}

EToroValidPins AToroGameMode::GetToroGameMode(AToroGameMode*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroGameMode>& Class)
{
	AToroGameMode* Obj = Cast<AToroGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

FGameplayTag AToroGameMode::GetZoneFromCharacter(const FGameplayTag& InCharacter) const
{
	return CharacterToZone.FindRef(InCharacter);
}

TSet<FGameplayTag> AToroGameMode::GetCharactersInZone(const FGameplayTag& InZone) const
{
	TSet<FGameplayTag> Characters;
	for (const TPair<FGameplayTag, FGameplayTag>& Pair : CharacterToZone)
	{
		if (Pair.Key.IsValid() && Pair.Value == InZone)
		{
			Characters.Add(Pair.Key);
		}
	}

	return Characters;
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
