// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameNarrative.h"
#include "Player/GamePlayer.h"

UGameNarrative* UGameNarrative::Get(const UObject* ContextObject)
{
	AGamePlayer* Player = AGamePlayer::Get<AGamePlayer>(ContextObject);
	return Player ? Player->Narrative : nullptr;
}
