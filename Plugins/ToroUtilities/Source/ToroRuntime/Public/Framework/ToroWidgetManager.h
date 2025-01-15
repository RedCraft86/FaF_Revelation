// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "UserWidgets/ToroUserWidget.h"
#include "ToroWidgetManager.generated.h"

/* Player State is repurposed as a Widget Manager */

UCLASS()
class TORORUNTIME_API AToroWidgetManager : public APlayerState
{
	GENERATED_BODY()

public:

	AToroWidgetManager();

	UPROPERTY(EditAnywhere, Category = Settings)
		TSet<TSubclassOf<UToroUserWidget>> DefaultWidgets;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Widget Manager"))
		static EToroValidPins GetToroWidgetManager(AToroWidgetManager*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroWidgetManager>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroWidgetManager>
	static T* Get(const UObject* WorldContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerState(WorldContextObject, PlayerIndex));
	}
	
	UFUNCTION(BlueprintCallable, Category = WidgetManager, meta = (DynamicOutputParam = "ReturnValue", DeterminesOutputType = "Class"))
		UToroUserWidget* FindOrAddWidget(const TSubclassOf<UToroUserWidget>& Class);
	
	UFUNCTION(BlueprintPure, Category = WidgetManager, meta = (DynamicOutputParam = "ReturnValue", DeterminesOutputType = "Class"))
		UToroUserWidget* FindWidget(const TSubclassOf<UToroUserWidget>& Class);

	template<typename T = UToroUserWidget>
	T* FindOrAddWidget(const TSubclassOf<UToroUserWidget>& Class) { return Cast<T>(FindOrAddWidget(Class)); }

	template<typename T = UToroUserWidget>
	T* FindWidget() { return Cast<T>(FindWidget(T::StaticClass())); }
	
protected:
	
	UPROPERTY(Transient)
		TSet<TObjectPtr<UToroUserWidget>> WidgetObjs;

	virtual void BeginPlay() override;
};
