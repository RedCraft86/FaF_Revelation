// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroPlayerController.h"
#include "ToroPlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

AToroPlayerController::AToroPlayerController()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	PlayerCameraManagerClass = AToroPlayerCameraManager::StaticClass();
}

EToroValidPins AToroPlayerController::GetToroPlayerController(AToroPlayerController*& OutObject,
	const UObject* WorldContextObject, const int32 PlayerIndex, const TSubclassOf<AToroPlayerController>& Class)
{
	AToroPlayerController* Obj = Cast<AToroPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, PlayerIndex));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

template <typename T>
T* AToroPlayerController::Get(const UObject* WorldContextObject, const int32 PlayerIndex)
{
	return Cast<T>(UGameplayStatics::GetPlayerController(WorldContextObject, PlayerIndex));
}

void AToroPlayerController::SetGameInputMode(const EGameInputMode InputMode, const bool bShowMouseCursor,
	const EMouseLockMode MouseLock, const bool bHideCursorOnCapture, UUserWidget* FocusWidget)
{
	SetInputModeData({InputMode, bShowMouseCursor, MouseLock, bHideCursorOnCapture, FocusWidget});
}

void AToroPlayerController::SetInputModeData(const FGameInputModeData& InputMode)
{
	InputModeData = InputMode;
	SetShowMouseCursor(InputModeData.ShouldShowMouse());
	SetInputMode(InputModeData.GetInputMode());
	FlushPressedKeys();
}

void AToroPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	InputModeData.ClearReferences();
	Super::EndPlay(EndPlayReason);
}
