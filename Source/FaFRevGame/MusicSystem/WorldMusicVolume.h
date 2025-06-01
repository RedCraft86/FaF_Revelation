// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "WorldMusicVolume.generated.h"

UCLASS()
class FAFREVGAME_API AWorldMusicVolume final : public AToroVolume
{
	GENERATED_BODY()

public:

	AWorldMusicVolume();

private:

	UPROPERTY(VisibleAnywhere, Category = Subobjects)
		TObjectPtr<UAudioComponent> MusicComponent;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
