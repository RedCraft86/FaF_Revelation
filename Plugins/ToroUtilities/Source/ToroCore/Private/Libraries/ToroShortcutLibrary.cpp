// Copyright (C) RedCraft86. All Rights Reserved.

#include "Libraries/ToroShortcutLibrary.h"
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
	ULevelSequencePlayer* Player = IsValid(Target) ? Target->GetSequencePlayer() : nullptr;
	if (Player && Callback.IsBound())
	{
		Player->OnFinished.AddUnique(Callback);
	}
}

void UToroShortcutLibrary::UnbindOnSequenceFinished(const ALevelSequenceActor* Target, const FRCBasicDelegate& Callback)
{
	ULevelSequencePlayer* Player = IsValid(Target) ? Target->GetSequencePlayer() : nullptr;
	if (Player && Callback.IsBound())
	{
		Player->OnFinished.Remove(Callback);
	}
}

void UToroShortcutLibrary::PlayAmbientSound(const AAmbientSound* Target, const float StartTime)
{
	if (UAudioComponent* Audio = IsValid(Target) ? Target->GetAudioComponent() : nullptr)
	{
		Audio->Play(StartTime);
	}
}

void UToroShortcutLibrary::StopAmbientSound(const AAmbientSound* Target, const float DelayTime)
{
	if (UAudioComponent* Audio = IsValid(Target) ? Target->GetAudioComponent() : nullptr)
	{
		Audio->StopDelayed(DelayTime);
	}
}

void UToroShortcutLibrary::FadeInAmbientSound(const AAmbientSound* Target, const float FadeTime, const float FadeVolume, const float StartTime)
{
	if (UAudioComponent* Audio = IsValid(Target) ? Target->GetAudioComponent() : nullptr)
	{
		Audio->FadeIn(FadeTime, FadeVolume, StartTime);
	}
}

void UToroShortcutLibrary::FadeOutAmbientSound(const AAmbientSound* Target, const float FadeTime, const float FadeVolume)
{
	if (UAudioComponent* Audio = IsValid(Target) ? Target->GetAudioComponent() : nullptr)
	{
		Audio->FadeOut(FadeTime, FadeVolume);
	}
}

void UToroShortcutLibrary::AdjustAmbientSoundVolume(const AAmbientSound* Target, const float FadeTime, const float FadeVolume)
{
	if (UAudioComponent* Audio = IsValid(Target) ? Target->GetAudioComponent() : nullptr)
	{
		Audio->AdjustVolume(FadeTime, FadeVolume);
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
