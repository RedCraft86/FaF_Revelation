// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "LegacyCameraShake.h"
#include "PlayerTypes.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EPlayerControlFlags
{
	PCF_None		= 0			UMETA(Hidden),
	PCF_Locked		= 1 << 0	UMETA(DisplayName = "Locked"),

	// Ability flags
	PCF_UseStamina	= 1 << 1	UMETA(DisplayName = "Use Stamina"),
	PCF_CanPause	= 1 << 2	UMETA(DisplayName = "Can Pause"),
	PCF_CanTurn		= 1 << 3	UMETA(DisplayName = "Can Turn"),
	PCF_CanMove		= 1 << 4	UMETA(DisplayName = "Can Walk"),
	PCF_CanRun		= 1 << 5	UMETA(DisplayName = "Can Run"),
	PCF_CanCrouch	= 1 << 6	UMETA(DisplayName = "Can Crouch"),
	PCF_CanLean		= 1 << 7	UMETA(DisplayName = "Can Lean"),
	PCF_CanInteract	= 1 << 8	UMETA(DisplayName = "Can Interact"),
	PCF_CanHide		= 1 << 9	UMETA(DisplayName = "Can Hide")
};
ENUM_CLASS_FLAGS(EPlayerControlFlags);
ENUM_RANGE_BY_FIRST_AND_LAST(EPlayerControlFlags, PCF_Locked, PCF_CanHide);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EPlayerStateFlags
{
	PSF_None		= 0			UMETA(Hidden),
	PSF_Run			= 1 << 0	UMETA(DisplayName = "Running"),
	PSF_Crouch		= 1 << 1	UMETA(DisplayName = "Crouching"),
	PSF_RunLocked	= 1 << 2	UMETA(DisplayName = "Stamina Punished"),
	PSF_Inventory	= 1 << 3	UMETA(DisplayName = "Using Inventory"),
	PSF_GuideBook	= 1 << 4	UMETA(DisplayName = "Reading Guide"),
	PSF_QuickTime	= 1 << 5	UMETA(DisplayName = "Playing QTE"),
	PSF_Inspect		= 1 << 6	UMETA(DisplayName = "Inspecting"),
	PSF_Hiding		= 1 << 7	UMETA(DisplayName = "Hiding"),
	PSF_Device		= 1 << 8	UMETA(DisplayName = "Using Device"),
	PSF_Tasking		= 1 << 9	UMETA(DisplayName = "Doing Tasks"),
};
ENUM_CLASS_FLAGS(EPlayerStateFlags);

UENUM(BlueprintType)
enum class EPlayerLeanState : uint8
{
	None,
	Left,
	Right
};

USTRUCT(BlueprintType)
struct FAFREVGAME_API FPlayerStamina
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Stamina, meta = (ClampMin = 10.0f, UIMin = 10.0f))
		float MaxStamina;

	UPROPERTY(EditAnywhere, Category = Stamina, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		FVector2D StaminaRates;

	FPlayerStamina(): MaxStamina(100.0f), StaminaRates(FVector2D::UnitVector), Delta(0.0f), Stamina(100.0f) {}

	float GetDelta() const { return Delta; }
	float GetPercent() const { return Stamina / MaxStamina; }
	bool IsEmpty() const { return FMath::IsNearlyZero(Stamina) || Stamina <= 0.0f; }
	bool IsFull() const { return FMath::IsNearlyEqual(Stamina, MaxStamina, 1.0f); }
	void SetPercent(const float InPercent) { Stamina = FMath::Lerp(0.0f, MaxStamina, InPercent); }
	void TickStamina(const bool bRunning)
	{
		Delta = bRunning ? -StaminaRates.X : StaminaRates.Y;
		Stamina = FMath::Clamp(Stamina + Delta, 0.0f, MaxStamina);
	}

private:

	float Delta;
	float Stamina;
};

USTRUCT(BlueprintType)
struct FAFREVGAME_API FPlayerFootsteps
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		FVector Intervals;

	UPROPERTY(EditAnywhere, Category = Footsteps)
		TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> WalkSounds;

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> RunSounds;

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> SneakSounds;

	FPlayerFootsteps(): Intervals(0.5f, 0.35f, 0.6f), TraceChannel(ECC_Visibility) {}

	USoundBase* GetFootstepSound(const int32 StateFlags, const EPhysicalSurface Surface) const
	{
		if (StateFlags & PSF_Run) return RunSounds.Find(Surface)
			? RunSounds.FindRef(Surface) : RunSounds.FindRef(SurfaceType_Default);
		
		if (StateFlags & PSF_Crouch) return SneakSounds.Find(Surface)
			? SneakSounds.FindRef(Surface) : SneakSounds.FindRef(SurfaceType_Default);
		
		return WalkSounds.Find(Surface)
			? WalkSounds.FindRef(Surface)
			: WalkSounds.FindRef(SurfaceType_Default);
	}

	float GetFootstepInterval(const int32 StateFlags) const
	{
		if (StateFlags & PSF_Run) return Intervals.Y;
		if (StateFlags & PSF_Crouch) return Intervals.Z;
		return Intervals.X;
	}
#if WITH_EDITOR
	void Validate()
	{
		if (const UEnum* Enum = StaticEnum<EPhysicalSurface>())
		{
			for(int32 i = 0; i < Enum->NumEnums(); i++)
			{
				const EPhysicalSurface Type = static_cast<EPhysicalSurface>(i);
				if (Type != SurfaceType_Default && Enum->HasMetaData(TEXT("Hidden"), i))
				{
					WalkSounds.Remove(TEnumAsByte(Type));
					RunSounds.Remove(TEnumAsByte(Type));
					SneakSounds.Remove(TEnumAsByte(Type));
					continue;
				}
				
				if (!WalkSounds.Contains(TEnumAsByte(Type))) WalkSounds.Add(TEnumAsByte(Type));
				if (!RunSounds.Contains(TEnumAsByte(Type))) RunSounds.Add(TEnumAsByte(Type));
				if (!SneakSounds.Contains(TEnumAsByte(Type))) SneakSounds.Add(TEnumAsByte(Type));
			}
		}
	}
#endif
};

UCLASS()
class FAFREVGAME_API UCamShake_Walk : public ULegacyCameraShake
{
	GENERATED_BODY()
	UCamShake_Walk();
};

UCLASS()
class FAFREVGAME_API UCamShake_Run : public ULegacyCameraShake
{
	GENERATED_BODY()
	UCamShake_Run();
};

namespace Player
{
	namespace Inputs
	{
		inline static FName Pause			= TEXT("Pause");
		inline static FName Back			= TEXT("Back");
		inline static FName Turn			= TEXT("Turn");
		inline static FName Move			= TEXT("Move");
		inline static FName Run				= TEXT("Run");
		inline static FName Crouch			= TEXT("Crouch");
		inline static FName Lean			= TEXT("Lean");
		inline static FName Inventory		= TEXT("Inventory");
		inline static FName HideQuests		= TEXT("HideQuests");
		inline static FName Interact		= TEXT("Interact");
		inline static FName Equipment		= TEXT("Equipment");

		inline static TSet All = { Pause, Back, Turn, Move, Run, Crouch, Lean,
			Inventory, HideQuests, Interact, Equipment };
	}

	namespace Keys
	{
		inline static FName Movement	= TEXT("Internal_Movement");
		inline static FName Leaning		= TEXT("Internal_Leaning");
		inline static FName Running		= TEXT("Internal_Running");
		inline static FName Crouching	= TEXT("Internal_Crouching");
		inline static FName Inspecting	= TEXT("Internal_Inspecting");
	}

	static float LeanToFloat(const EPlayerLeanState& State)
	{
		switch (State)
		{
		case EPlayerLeanState::Left: return -1.0f;
		case EPlayerLeanState::Right: return 1.0f;
		default: return 0.0f;
		}
	}

	inline static int32 DefaultControls = PCF_UseStamina | PCF_CanPause | PCF_CanTurn | PCF_CanMove
		| PCF_CanRun | PCF_CanCrouch | PCF_CanLean | PCF_CanInteract | PCF_CanHide;

	inline static int32 LockingStates = PSF_Inventory | PSF_GuideBook
		| PSF_QuickTime | PSF_Inspect | PSF_Hiding | PSF_Device;
}