// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Camera/CameraActor.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerChar/GamePlayerChar.h"
#include "Framework/ToroCameraManager.h"
#include "Framework/ToroWidgetManager.h"
#include "MiscActors/ToroSequenceActor.h"
#include "UserWidgets/GameOverWidget.h"
#include "GameOverSequence.generated.h"

UCLASS()
class FAFREVGAME_API AGameOverSequence final : public AToroSequenceActor
{
	GENERATED_BODY()

public:

	AGameOverSequence(const FObjectInitializer& Init): Super(Init) {}

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<ACameraActor> Camera;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Categories = "Character"))
		FGameplayTag CharacterID;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "ContextObject"))
	static void PlayGameOver(const UObject* ContextObject, UPARAM(meta = (Categories = "Character")) const FGameplayTag& InCharacter)
	{
		AToroPlayerController* Controller = AToroPlayerController::Get(ContextObject);
		AToroCameraManager* CamManager = AToroCameraManager::Get(ContextObject);
		if (Controller && CamManager)
		{
			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsOfClass(ContextObject, StaticClass(), Actors);
			for (AActor* Actor : Actors)
			{
				AGameOverSequence* Sequence = Cast<AGameOverSequence>(Actor);
				if (Sequence && Sequence->CharacterID == InCharacter)
				{
					Controller->SetViewTarget(Sequence->Camera);
					CamManager->SetGameCameraCutThisFrame();
					Sequence->Play();
					return;
				}
			}
		}
	}

	virtual void OnFinished() override
	{
		OnFinishedEvent.Broadcast();
		if (AToroWidgetManager* WidgetManager = AToroWidgetManager::Get(this))
		{
			if (UGameOverWidget* OverWidget = WidgetManager->FindWidget<UGameOverWidget>())
			{
				OverWidget->ActivateWidget();
			}
		}
		if (AGamePlayerChar* Player = AGamePlayerChar::Get<AGamePlayerChar>(this))
		{
			Player->Teleport(FVector::ZeroVector, FRotator::ZeroRotator);
		}
	}

#if WITH_EDITOR
private:
	virtual bool CanEditChange(const FProperty* InProperty) const override
	{
		const bool bSuper = Super::CanEditChange(InProperty);
		return bSuper && InProperty
			&& InProperty->GetFName() != "bSkippable"
			&& InProperty->GetFName() != "bLockPlayer";
	}
	virtual void OnConstruction(const FTransform& Transform) override
	{
		Super::OnConstruction(Transform);
		bLockPlayer = false;
		bSkippable = false;
	}
#endif
};
