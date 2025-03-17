// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCore.h"
#include "ECFHandle.h"
#include "ToroDataAsset.h"
#include "GameplayTagContainer.h"
#include "LocalSoundTypes.generated.h"

class AToroMusicManager;

UE_DECLARE_GAMEPLAY_TAG_BASE(LocalSound)

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FLocalSoundEntry
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = LocalSound)
	FString Label = TEXT("");
#endif

	UPROPERTY(EditAnywhere, Category = LocalSound)
		TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = LocalSound)
		FVector Fades;
	
	UPROPERTY(EditAnywhere, Category = LocalSound)
		float Volume;

	UPROPERTY(EditAnywhere, Category = LocalSound)
		FVector2D StartRange;

	FLocalSoundEntry() : Fades(1.0f), Volume(1.0f), StartRange(0.0f) {}
	bool IsValidData() const { return IsValid(Sound.LoadSynchronous()); }
	float GetStartTime() const
	{
		if (FMath::IsNearlyEqual(StartRange.X, StartRange.Y)) return StartRange.X;
		return FMath::RandRange(StartRange.GetMin(), StartRange.GetMax());
	}
#if WITH_EDITOR
	void Update()
	{
		Sound.LoadSynchronous();
		Fades = FVector::Max(FVector::OneVector, Fades);
		StartRange = FVector2D::Max(FVector2D::ZeroVector, StartRange);
		Volume = FMath::Max(0.1f, Volume);
	}
#endif
};

UCLASS()
class TOROUTILITIES_API ULocalSoundDatabase : public UToroDatabase
{
	GENERATED_BODY()

public:
	
	ULocalSoundDatabase() {}

	UPROPERTY(EditAnywhere, Category = LocalSound, meta = (ForceInlineRow, Categories = "LocalSound", TitleProperty = "Label"))
		TMap<FGameplayTag, FLocalSoundEntry> Entries;

	bool IsKeyValid(const FGameplayTag& Key) const;
	const FLocalSoundEntry* Find(const FGameplayTag& Key) const;

	static bool IsValidKey(const FGameplayTag& Key);
	static FLocalSoundEntry Get(const FGameplayTag& Key);

#if WITH_EDITOR
	virtual int32 GetValidData() const override;
	virtual int32 GetTotalData() const override { return Entries.Num(); }
protected:
	virtual void UpdateData() override;
#endif
};

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FLocalSoundLayer
{
	GENERATED_BODY()

private:
	
	FECFHandle FadeHandle;
	FECFHandle PauseHandle;
	UPROPERTY() bool bPaused;
	UPROPERTY() bool bStopping;
	UPROPERTY() bool bAutoDestroy;
	UPROPERTY() FGameplayTag SoundID;
	UPROPERTY(Transient) TObjectPtr<UAudioComponent> Component;
	UPROPERTY(Transient) TSet<TObjectPtr<const UObject>> Instigators;
	UPROPERTY(Transient) TObjectPtr<AToroMusicManager> Owner;

public:
	
	void Stop();
	void Restart();
	void SetPaused(const bool bInPaused);
	void AddInstigator(const UObject* InObject);
	void RemoveInstigator(const UObject* InObject);
	
	bool IsLooping() const;
	bool IsValidLayer() const;
	bool CanRunFunctions() const;
	void Initialize(AToroMusicManager* InOwner, const UObject* Instigator);
	void OnAudioFinished(UAudioComponent* Comp);

	FLocalSoundLayer() : bPaused(false), bStopping(false), bAutoDestroy(false) {}
	FLocalSoundLayer(const FGameplayTag& InSoundID)
		: bPaused(false), bStopping(false), bAutoDestroy(false), SoundID(InSoundID)
	{}
};