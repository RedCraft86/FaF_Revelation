// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Kismet/GameplayStatics.h"
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

protected:

	UPROPERTY(Transient) TSet<TObjectPtr<UToroWidget>> WidgetObjs;
	UPROPERTY(Transient) TObjectPtr<AToroPlayerController> PlayerController;

	virtual void BeginPlay() override;
};
