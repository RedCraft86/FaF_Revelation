// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "FootstepTypes.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UFootstepDatabase final : public UDataAsset
{
	GENERATED_BODY()

public:

	UFootstepDatabase() {}

	UPROPERTY(EditAnywhere, Category = Footsteps)
		TObjectPtr<USoundBase> Default;

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ReadOnlyKeys))
		TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> Walking;

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ReadOnlyKeys))
		TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> Running;

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ReadOnlyKeys))
		TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> Crouching;

	USoundBase* GetSound(const EPhysicalSurface Surface, const bool bRun, const bool bCrouch) const
	{
		USoundBase* Sound;
		if (bRun) Sound = Running.FindRef(Surface);
		else if (bCrouch) Sound = Crouching.FindRef(Surface);
		else Sound = Walking.FindRef(Surface);
		return Sound ? Sound : Default.Get();
	}

#if WITH_EDITOR
	int32 ValidEntries() const;
	int32 AllEntries() const { return Walking.Num() + Running.Num() + Crouching.Num() + 1; }
private:
	void UpdateEntries();
	virtual void PostLoad() override;
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};