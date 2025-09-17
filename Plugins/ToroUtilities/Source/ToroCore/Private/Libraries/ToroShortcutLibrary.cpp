// Copyright (C) RedCraft86. All Rights Reserved.

#include "Libraries/ToroShortcutLibrary.h"
#include "Engine/LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"

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
	if (IsValid(Target))
	{
		Target->Tags.AddUnique(InTag);
	}
}

void UToroShortcutLibrary::RemoveActorTag(AActor* Target, const FName InTag)
{
	if (IsValid(Target))
	{
		Target->Tags.Remove(InTag);
	}
}

void UToroShortcutLibrary::RestartLevel(const UObject* ContextObject, const FString Options)
{
	UGameplayStatics::OpenLevel(ContextObject, *UGameplayStatics::GetCurrentLevelName(ContextObject), true, Options);
}

void UToroShortcutLibrary::CallRemoteEvent(UObject* ContextObject, const FName EventName)
{
	if (EventName.IsNone()) return;
	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (ALevelScriptActor* LSA = World ? World->GetLevelScriptActor() : nullptr)
	{
		LSA->RemoteEvent(EventName);
	}
}
