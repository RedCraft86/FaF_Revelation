// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ECFHandle.h"
#include "ExecPinEnums.h"
#include "MetasoundSource.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "MusicSystem/LocalSoundTypes.h"
#include "GameFramework/GameStateBase.h"
#include "ToroMusicManager.generated.h"

/* Game State is repurposed as a Music Manager */
UCLASS()
class TORORUNTIME_API AToroMusicManager : public AGameStateBase
{
	GENERATED_BODY()

public:

	AToroMusicManager();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> MainThemeComponent;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Music Manager"))
		static EToroValidPins GetMusicManager(AToroMusicManager*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroMusicManager>& Class);

	template <typename T = AToroMusicManager>
	static T* Get(const UObject* WorldContextObject)
	{
		return Cast<T>(UGameplayStatics::GetGameState(WorldContextObject));
	}

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		bool ChangeMainTheme(UMetaSoundSource* NewTheme);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = MusicManager)
		void SetThemeIntensity(const float InIntensity) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = MusicManager)
		void SetThemeState(const uint8 InState) const;
	
	UFUNCTION(BlueprintCallable, Category = MusicManager, meta = (DefaultToSelf = "InInstigator"))
		bool PlayLayer(const UObject* InInstigator, UPARAM(meta = (Categories = "LocalSounds")) const FGameplayTag InSoundID);

	UFUNCTION(BlueprintCallable, Category = MusicManager, meta = (DefaultToSelf = "InInstigator"))
		bool StopLayer(const UObject* InInstigator, UPARAM(meta = (Categories = "LocalSounds")) const FGameplayTag InSoundID);

	UFUNCTION(BlueprintCallable, Category = MusicManager, meta = (DefaultToSelf = "InInstigator"))
		bool StopLayerIfLooping(const UObject* InInstigator, UPARAM(meta = (Categories = "LocalSounds")) const FGameplayTag InSoundID);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		bool RestartLayer(UPARAM(meta = (Categories = "LocalSounds")) const FGameplayTag InSoundID);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		bool SetLayerPaused(UPARAM(meta = (Categories = "LocalSounds")) const FGameplayTag InSoundID, const bool bPaused);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void CleanLocalSoundTracks();

protected:

	FECFHandle ChangeHandle;
	UPROPERTY(Transient) TObjectPtr<UMetaSoundSource> MainTheme;
	UPROPERTY(Transient) TMap<FGameplayTag, FLocalSoundLayer> LocalSoundLayers;

	IAudioParameterControllerInterface* GetSoundParamInterface() const;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
