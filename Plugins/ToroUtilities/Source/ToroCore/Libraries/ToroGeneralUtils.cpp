// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroGeneralUtils.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#include "ShaderPipelineCache.h"

EWindowsDialogueResult UToroGeneralUtils::OpenWindowsDialogue(const FString Title, const FString Message,
	const EWindowsDialogueType Type, const EWindowsDialogueIcon Icon)
{
	return WindowsHelpers::OpenDialogue(Title, Message, Type, Icon);
}

void UToroGeneralUtils::TriggerVirtualKey(const FKey InKey, const EVirtualKeyTriggerType Type, const bool bRepeat)
{
	WindowsHelpers::VirtualKey(InKey, Type, bRepeat);
}

bool UToroGeneralUtils::IsActorInScreen(const AActor* Target, const float MaxDistance, const bool bOriginOnly,
	const bool bLineTraceCheck, const FActorBoundsCheckParams& TraceCheckParams)
{
	if (!IsValid(Target) || MaxDistance <= 0
		|| (bLineTraceCheck && TraceCheckParams.BoundingBoxLerp.Size() <= 0))
	{
		return false;
	}

	APlayerController* Controller = UGameplayStatics::GetPlayerController(Target, 0);
	if (!Controller || !Controller->GetLocalPlayer()) return false;
	
	ULocalPlayer* LocalPlayer = Controller->GetLocalPlayer();
	FSceneViewFamilyContext ViewFamily(
		FSceneViewFamily::ConstructionValues(LocalPlayer->ViewportClient->Viewport, Target->GetWorld()->Scene,
			LocalPlayer->ViewportClient->EngineShowFlags).SetRealtimeUpdate(true)
	);
	
	FVector ViewLocation;
	FRotator ViewRotation;
	FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);
	if (FVector::Distance(ViewLocation, Target->GetActorLocation()) > MaxDistance)
	{
		return false;
	}

	FVector Origin, BoxExtent = FVector::ZeroVector;
	Target->GetActorBounds(false, Origin, BoxExtent, false);
	if (!SceneView || !SceneView->ViewFrustum.IntersectBox(Origin, BoxExtent))
	{
		return false;
	}
	
	TArray<FVector> TestVectors;
	TestVectors.Add(Target->GetActorLocation());

	if (!bOriginOnly)
	{
		TArray<FVector> BoundingBoxVertices = UPrimitiveDataLibrary::GetBoundingBoxVertices(
			Target, TraceCheckParams.bOnlyCollidingComponents,
			TraceCheckParams.bIncludeFromChildActors, Origin, BoxExtent
		);
		
		TestVectors = TraceCheckParams.ProcessVertices(BoundingBoxVertices, Origin);
	}

	bool bPassedScreenTest = false;
	for (int i = 0; i < TestVectors.Num(); i++)
	{
		FVector2D ScreenLoc;
		FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(Controller);
		
		if (!Controller->ProjectWorldLocationToScreen(TestVectors[i], ScreenLoc)) continue;
		if (ScreenLoc.X >= 0 && ScreenLoc.X <= ViewportSize.X && ScreenLoc.Y >= 0 && ScreenLoc.Y <= ViewportSize.Y)
		{
			bPassedScreenTest = true;
			break;
		}
	}

	if (!bPassedScreenTest) return false;
	if (!bLineTraceCheck) return true;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Target);
	QueryParams.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(Target, 0));

	FHitResult HitResult;
	for (int i = 0; i < TestVectors.Num(); i++)
	{
		if (!Target->GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation,
			TestVectors[i], TraceCheckParams.LineTraceChannel, QueryParams))
			return true;
	}

	return false;
}

bool UToroGeneralUtils::IsLocationInFront(const AActor* Target, const FVector& Location)
{
	if (!IsValid(Target)) return false;
	FVector DotVec = Location - Target->GetActorLocation(); DotVec.Normalize();
	return FVector::DotProduct(Target->GetActorForwardVector(), DotVec) > 0.0f;
}

bool UToroGeneralUtils::IsActorInFront(const AActor* Target, const AActor* ActorToTest)
{
	if (!IsValid(Target) || !IsValid(ActorToTest)) return false;
	return IsLocationInFront(Target, ActorToTest->GetActorLocation());
}

UWorld* UToroGeneralUtils::GetPlayWorld()
{
	return GEngine->GetCurrentPlayWorld();
}

bool UToroGeneralUtils::IsInEditor()
{
#if WITH_EDITOR
	return !FApp::IsGame();
#else
	return false;
#endif
}

void UToroGeneralUtils::RestartLevel(const UObject* ContextObject, const FString Options)
{
	UGameplayStatics::OpenLevel(ContextObject, *UGameplayStatics::GetCurrentLevelName(ContextObject), true, Options);
}

void UToroGeneralUtils::CallLocalEvent(UObject* Target, const FName EventName)
{
	if (!IsValid(Target) || EventName.IsNone()) return;
	
	FOutputDeviceNull Ar;
	Target->CallFunctionByNameWithArguments(*EventName.ToString(), Ar, nullptr, true);
}

void UToroGeneralUtils::ForceGarbageCollection()
{
	if (GEngine)
	{
#if WITH_EDITOR
		GEngine->ForceGarbageCollection(false);
#else
		GEngine->ForceGarbageCollection(true);
#endif
	}
}
