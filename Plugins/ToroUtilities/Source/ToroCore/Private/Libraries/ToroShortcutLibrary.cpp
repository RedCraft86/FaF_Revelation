// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroShortcutLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "Sound/AmbientSound.h"

void UToroShortcutLibrary::ClearCameraFade(const UObject* ContextObject)
{
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(ContextObject, 0))
	{
		PCM->StopCameraFade();
	}
}

void UToroShortcutLibrary::SetCameraFade(const UObject* ContextObject, const float Alpha, const FLinearColor Color, const bool bFadeAudio)
{
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(ContextObject, 0))
	{
		PCM->SetManualCameraFade(Alpha, Color, bFadeAudio);
	}
}

void UToroShortcutLibrary::StartCameraFade(UObject* ContextObject, const float FromAlpha, const float ToAlpha,
	const float Duration, const FLinearColor Color, const bool bShouldFadeAudio, const bool bHoldWhenFinished)
{
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(ContextObject, 0))
	{
		PCM->StartCameraFade(FromAlpha, ToAlpha, Duration, Color, bShouldFadeAudio, bHoldWhenFinished);
	}
}

void UToroShortcutLibrary::SetViewTarget(UObject* ContextObject, AActor* NewTarget, const float BlendTime,
	const EViewTargetBlendFunction BlendFunc, const float BlendExp, const bool bLockOutgoing)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(ContextObject, 0))
	{
		PC->SetViewTargetWithBlend(NewTarget, BlendTime, BlendFunc, BlendExp, bLockOutgoing);
	}
}

void UToroShortcutLibrary::AddActorTag(AActor* Target, const FName InTag)
{
	if (IsValid(Target)) Target->Tags.AddUnique(InTag);
}

void UToroShortcutLibrary::RemoveActorTag(AActor* Target, const FName InTag)
{
	if (IsValid(Target)) Target->Tags.Remove(InTag);
}

void UToroShortcutLibrary::PlayLevelSequence(const ALevelSequenceActor* Target)
{
	if (ULevelSequencePlayer* Player = IsValid(Target) ? Target->GetSequencePlayer() : nullptr)
	{
		Player->Play();
	}
}

void UToroShortcutLibrary::StopLevelSequence(const ALevelSequenceActor* Target)
{
	if (ULevelSequencePlayer* Player = IsValid(Target) ? Target->GetSequencePlayer() : nullptr)
	{
		Player->Stop();
	}
}

void UToroShortcutLibrary::SkipLevelSequence(const ALevelSequenceActor* Target)
{
	if (ULevelSequencePlayer* Player = IsValid(Target) ? Target->GetSequencePlayer() : nullptr)
	{
		Player->GoToEndAndStop();
	}
}

void UToroShortcutLibrary::BindOnSequenceFinished(const ALevelSequenceActor* Target, const FRCBasicDelegate& Callback)
{
	if (!Callback.IsBound()) return;
	if (ULevelSequencePlayer* Player = IsValid(Target) ? Target->GetSequencePlayer() : nullptr)
	{
		Player->OnFinished.AddUnique(Callback);
	}
}

void UToroShortcutLibrary::UnbindOnSequenceFinished(const ALevelSequenceActor* Target, const FRCBasicDelegate& Callback)
{
	if (!Callback.IsBound()) return;
	if (ULevelSequencePlayer* Player = IsValid(Target) ? Target->GetSequencePlayer() : nullptr)
	{
		Player->OnFinished.Remove(Callback);
	}
}

UActorComponent* UToroShortcutLibrary::AddActorInstanceComponent(AActor* Target, const TSubclassOf<UActorComponent> InClass)
{
#if WITH_EDITOR
	if (Target && InClass) 
	{
		UActorComponent* NewComponent = NewObject<UActorComponent>(Target, InClass);
		NewComponent->RegisterComponent();
		Target->AddInstanceComponent(NewComponent);
		return NewComponent;
	}
#endif
	return nullptr;
}
