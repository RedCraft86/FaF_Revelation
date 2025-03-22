// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ECFHandle.h"
#include "ExecPinEnums.h"
#include "MetasoundSource.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "DataTypes/LocalSoundTypes.h"
#include "GameFramework/GameStateBase.h"
#include "ToroMusicManager.generated.h"

UCLASS()
class TOROUTILITIES_API AToroMusicManager : public AGameStateBase
{
	GENERATED_BODY()

public:

	AToroMusicManager();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> ThemeComponent;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "ContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Music Manager"))
	static EToroValidPins GetMusicManager(AToroMusicManager*& OutObject, const UObject* ContextObject, const TSubclassOf<AToroMusicManager>& Class);

	template <typename T = AToroMusicManager>
	static T* Get(const UObject* ContextObject)
	{
		return Cast<T>(UGameplayStatics::GetGameState(ContextObject));
	}

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		bool ChangeMainTheme(UMetaSoundSource* NewTheme);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = MusicManager)
		void SetThemeMuted(const bool bMuted, const float Duration = 1.0f);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = MusicManager)
		void SetDipAudio(const bool bDipAudio) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = MusicManager)
		void SetHidingState(const bool bHiding) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = MusicManager)
		void SetThemeState(const uint8 InState) const;
	
	UFUNCTION(BlueprintCallable, Category = MusicManager, meta = (DefaultToSelf = "InInstigator"))
		bool PlayLayer(const UObject* InInstigator, UPARAM(meta = (Categories = "LocalSound")) const FGameplayTag InSoundID);

	UFUNCTION(BlueprintCallable, Category = MusicManager, meta = (DefaultToSelf = "InInstigator"))
		bool StopLayer(const UObject* InInstigator, UPARAM(meta = (Categories = "LocalSound")) const FGameplayTag InSoundID);

	UFUNCTION(BlueprintCallable, Category = MusicManager, meta = (DefaultToSelf = "InInstigator"))
		bool StopLayerIfLooping(const UObject* InInstigator, UPARAM(meta = (Categories = "LocalSound")) const FGameplayTag InSoundID);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		bool RestartLayer(UPARAM(meta = (Categories = "LocalSound")) const FGameplayTag InSoundID);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		bool SetLayerPaused(UPARAM(meta = (Categories = "LocalSound")) const FGameplayTag InSoundID, const bool bPaused);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void CleanLocalSoundTracks();

protected:

	FECFHandle MuteHandle;
	FECFHandle ChangeHandle;
	UPROPERTY() bool bThemeMuted;
	UPROPERTY(Transient) TObjectPtr<UMetaSoundSource> MainTheme;
	UPROPERTY(Transient) TMap<FGameplayTag, FLocalSoundLayer> LocalSoundLayers;

	IAudioParameterControllerInterface* GetSoundParamInterface() const;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
