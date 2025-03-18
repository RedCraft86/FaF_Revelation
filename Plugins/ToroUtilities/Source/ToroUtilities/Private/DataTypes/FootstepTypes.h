// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroDataAsset.h"
#include "FootstepTypes.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class TOROUTILITIES_API UFootstepDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:

	UFootstepDatabase() {}
	
	UPROPERTY(EditAnywhere, Category = Footsteps)
		TObjectPtr<USoundBase> Default;

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ReadOnlyKeys))
		TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> Surfaces;

	USoundBase* GetSound(const EPhysicalSurface Surface) const
	{
		USoundBase* Sound = Surfaces.FindRef(Surface);
		return Sound ? Sound : Default;
	}

#if WITH_EDITOR
	virtual int32 GetValidData() const override;
	virtual int32 GetTotalData() const override { return 1 + Surfaces.Num(); }
protected:
	virtual void UpdateData() override;
#endif
};
