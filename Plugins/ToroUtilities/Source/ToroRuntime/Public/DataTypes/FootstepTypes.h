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
private:
	void UpdateEntries()
	{
		if (FApp::IsGame()) return;
		if (const UEnum* Enum = StaticEnum<EPhysicalSurface>())
		{
			for(int32 i = 1; i < Enum->NumEnums(); i++)
			{
				const EPhysicalSurface Type = static_cast<EPhysicalSurface>(i);
				if (Enum->HasMetaData(TEXT("Hidden"), i))
				{
					Walking.Remove(TEnumAsByte(Type));
					Running.Remove(TEnumAsByte(Type));
					Crouching.Remove(TEnumAsByte(Type));
				}
				else 
				{
					if (!Walking.Contains(TEnumAsByte(Type))) Walking.Add(TEnumAsByte(Type));
					if (!Running.Contains(TEnumAsByte(Type))) Running.Add(TEnumAsByte(Type));
					if (!Crouching.Contains(TEnumAsByte(Type))) Crouching.Add(TEnumAsByte(Type));
				}
			}
		}
	}

	virtual void PostLoad() override
	{
		Super::PostLoad();
		UpdateEntries();
	}

	virtual void PostInitProperties() override
	{
		Super::PostLoad();
		UpdateEntries();
	}

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		UpdateEntries();
	}
#endif
};