// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "ToroWidgetManager.generated.h"

UCLASS()
class TOROUTILITIES_API AToroWidgetManager : public APlayerState
{
	GENERATED_BODY()

public:

	AToroWidgetManager();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "ContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Widget Manager"))
	static EToroValidPins GetToroWidgetManager(AToroWidgetManager*& OutObject, const UObject* ContextObject, const TSubclassOf<AToroWidgetManager>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroWidgetManager>
	static T* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerState(ContextObject, PlayerIndex));
	}

	UFUNCTION(BlueprintCallable, Category = WidgetManager, meta = (DynamicOutputParam = "ReturnValue", DeterminesOutputType = "Class"))
		UUserWidget* FindOrAddWidget(const TSubclassOf<UUserWidget> Class);

	UFUNCTION(BlueprintPure, Category = WidgetManager, meta = (DynamicOutputParam = "ReturnValue", DeterminesOutputType = "Class"))
		UUserWidget* FindWidget(const TSubclassOf<UUserWidget> Class);

	template<typename T>
	T* FindWidget() { return Cast<T>(FindWidget(T::StaticClass())); }

protected:

	UPROPERTY(Transient) TSet<TObjectPtr<UUserWidget>> Widgets;
	virtual void BeginPlay() override;
};
