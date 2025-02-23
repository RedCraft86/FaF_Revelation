// Copyright (C) RedCraft86. All Rights Reserved.

#include "Characters/ToroPlayerBase.h"
#include "Framework/ToroPlayerController.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_Player, "Characters.Player");

AToroPlayerBase::AToroPlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	CharacterID = Tag_Player;
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
