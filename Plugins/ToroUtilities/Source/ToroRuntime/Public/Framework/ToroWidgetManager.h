// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InputAction.h"
#include "ExecPinEnums.h"
#include "Kismet/GameplayStatics.h"
#include "DataTypes/MessagingTypes.h"
#include "GameFramework/PlayerState.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "ToroWidgetManager.generated.h"

/* Player State is repurposed as a Widget Manager */

UCLASS()
class TORORUNTIME_API AToroWidgetManager : public APlayerState
{
	GENERATED_BODY()

public:

	AToroWidgetManager();

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Widget Manager"))
		static EToroValidPins GetToroWidgetManager(AToroWidgetManager*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroWidgetManager>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroWidgetManager>
	static T* Get(const UObject* WorldContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerState(WorldContextObject, PlayerIndex));
	}

	UFUNCTION(BlueprintCallable, Category = WidgetManager, meta = (DynamicOutputParam = "ReturnValue", DeterminesOutputType = "Class"))
		UToroWidget* FindOrAddWidget(const TSubclassOf<UToroWidget> Class);
	
	UFUNCTION(BlueprintPure, Category = WidgetManager, meta = (DynamicOutputParam = "ReturnValue", DeterminesOutputType = "Class"))
		UToroWidget* FindWidget(const TSubclassOf<UToroWidget> Class);
	
	template<typename T>
	T* FindWidget()
	{
		return Cast<T>(FindWidget(T::StaticClass()));
	}

	UFUNCTION(BlueprintCallable, Category = Messaging)
		void QueueSmallNotice(const FSimpleMessageData& NoticeData, const bool bResetQueue = false);

	UFUNCTION(BlueprintCallable, Category = Messaging)
		void QueueLargeNotice(const FSimpleMessageData& NoticeData, const bool bResetQueue = false);

	UFUNCTION(BlueprintCallable, Category = Messaging)
		void QueueSubtitles(const TArray<FSimpleSubtitleData>& Subtitles, const bool bOverride = false);

	UFUNCTION(BlueprintCallable, Category = Messaging)
		void QueueSubtitle(const FSimpleSubtitleData& SubtitleData, const bool bOverride = false);

	UFUNCTION(BlueprintCallable, Category = Messaging)
		void AddControlEntry(const UInputAction* InAction);

	UFUNCTION(BlueprintCallable, Category = Messaging)
		void RemoveControlEntry(const UInputAction* InAction);

	UFUNCTION(BlueprintCallable, Category = GuidePage)
		void QueueGuidePage(UPARAM(meta = (Categories = "GuidePages")) const FGameplayTag PageID);

	UFUNCTION(BlueprintCallable, Category = GuidePage)
		void QueueGuidePages(UPARAM(meta = (Categories = "GuidePages")) const TArray<FGameplayTag>& PageIDs);

protected:

	UPROPERTY(Transient) TSet<TObjectPtr<UToroWidget>> WidgetObjs;
	UPROPERTY(Transient) TObjectPtr<AToroPlayerController> PlayerController;

	virtual void BeginPlay() override;
};
