// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/LightComponent.h"
#include "ToroLightEntry.generated.h"

USTRUCT(BlueprintType)
struct TOROSYSTEMS_API FToroLightEntry
{
	GENERATED_BODY()

	static inline FVector4 OffColor = FVector4(1.0f, 1.0f, 1.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightEntry)
		TObjectPtr<ULightComponent> Light;

	// Key: Mesh Component | Value: Should the mesh turn invisible when off?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightEntry)
		TMap<TObjectPtr<UStaticMeshComponent>, bool> Meshes;

	FToroLightEntry() {}
	void UpdateLight(const bool bState, const float Multiplier, const float Fresnel,
		const uint8 Flicker, UMaterialInterface* FlickerMat) const
	{
		Light->SetVisibility(bState);
		Light->SetLightFunctionMaterial(FlickerMat);

		FVector4 Color = Light->GetLightColor();
		Color.W = Light->Intensity * Multiplier;
		for (const TPair<TObjectPtr<UStaticMeshComponent>, bool>& Mesh : Meshes)
		{
			if (!Mesh.Key) continue;
			if (Mesh.Value) // Means we should unhide/hide when turning on/off
			{
				Mesh.Key->SetVisibility(bState);
			}

#if WITH_EDITOR
			if (!FApp::IsGame())
			{
				Mesh.Key->SetDefaultCustomPrimitiveDataVector4(0, bState ? Color : OffColor); // 0..3 (RGB + Intensity)
				Mesh.Key->SetDefaultCustomPrimitiveDataFloat(4, Fresnel);
				Mesh.Key->SetDefaultCustomPrimitiveDataFloat(5, Flicker);
				continue;
			}
#endif

			Mesh.Key->SetCustomPrimitiveDataVector4(0, bState ? Color : OffColor); // 0..3 (RGB + Intensity)
			Mesh.Key->SetCustomPrimitiveDataFloat(4, Fresnel);
			Mesh.Key->SetCustomPrimitiveDataFloat(5, Flicker);
		}
	}
};
