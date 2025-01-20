// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ECFHandle.h"
#include "ExecPinEnums.h"
#include "MetasoundSource.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "GameFramework/GameStateBase.h"
#include "ToroMusicManager.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FOneShotMusicLayer
{
	GENERATED_BODY()

	friend class AToroMusicManager;
	
	FECFHandle FadeHandle;
	FECFHandle PauseHandle;
	UPROPERTY() bool bPaused;
	UPROPERTY() FVector2D Start;
	UPROPERTY() bool bAutoDestroy;
	UPROPERTY(Transient) TObjectPtr<UAudioComponent> Component;
	UPROPERTY(Transient) TObjectPtr<AToroMusicManager> Owner;
	
	void Stop(const float FadeTime);
	void Restart(const float FadeTime);
	void SetPaused(const float FadeTime, const bool bInPaused);
	
	bool CanRunFunctions() const;
	void Initialize(const float FadeTime);
	void OnAudioFinished(UAudioComponent* Comp);

	FOneShotMusicLayer() : bPaused(false), Start(0.0f), bAutoDestroy(false) {}
};

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
	
	UFUNCTION(BlueprintCallable, Category = MusicManager)
		bool PlayLayer(USoundBase* Sound, const float FadeTime = 1.0f, const float Volume = 1.0f, const FVector2D& StartRange = FVector2D::ZeroVector);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		bool StopLayer(const USoundBase* Sound, const float FadeTime = 1.0f);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		bool RestartLayer(const USoundBase* Sound, const float FadeTime = 1.0f);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		bool SetLayerPaused(const USoundBase* Sound, const float FadeTime = 1.0f, const bool bPaused = true);

	UFUNCTION(BlueprintCallable, Category = MusicManager)
		void CleanOneShotTracks();

protected:

	FECFHandle ChangeHandle;
	UPROPERTY(Transient) TObjectPtr<UMetaSoundSource> MainTheme;
	UPROPERTY(Transient) TMap<TObjectPtr<USoundBase>, FOneShotMusicLayer> OneShotLayers;

	IAudioParameterControllerInterface* GetSoundParamInterface() const;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
