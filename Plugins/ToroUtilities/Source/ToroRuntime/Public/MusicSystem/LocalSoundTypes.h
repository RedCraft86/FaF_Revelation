// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ECFHandle.h"
#include "ToroDatabase.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "LocalSoundTypes.generated.h"

class AToroMusicManager;

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FLocalSoundEntry
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
#if WITH_EDITOR
	void Update();
#endif

	bool IsValidData() const { return IsValid(Sound.LoadSynchronous()); }
	float GetStartTime() const
	{
		if (FMath::IsNearlyEqual(StartRange.X, StartRange.Y)) return StartRange.X;
		return FMath::RandRange(StartRange.GetMin(), StartRange.GetMax());
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API ULocalSoundDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:
	
	ULocalSoundDatabase() {}

	UPROPERTY(EditAnywhere, Category = LocalSound, meta = (ForceInlineRow, Categories = "LocalSounds", TitleProperty = "Label"))
		TMap<FGameplayTag, FLocalSoundEntry> Entries;

	bool IsKeyValid(const FGameplayTag& Key) const;

	static bool IsValidKey(const FGameplayTag& Key);
	static FLocalSoundEntry Get(const FGameplayTag& Key);
	
#if WITH_EDITOR
	virtual int32 GetTotalData() const override;
	virtual int32 GetValidData() const override;
protected:
	virtual void UpdateData() override;
#endif
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FLocalSoundLayer
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