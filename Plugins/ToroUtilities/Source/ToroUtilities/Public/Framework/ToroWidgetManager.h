// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/HUD.h"
#include "Helpers/ClassGetters.h"
#include "ToroWidgetManager.generated.h"

UCLASS()
class TOROUTILITIES_API AToroWidgetManager : public AHUD
{
	GENERATED_BODY()

public:

	AToroWidgetManager();

	template <typename T = AToroWidgetManager>
	static T* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		const APlayerController* PC = UGameplayStatics::GetPlayerController(ContextObject, PlayerIndex);
		return PC ? Cast<T>(PC->GetHUD()) : nullptr;
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
