// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "QTEInstance.h"
#include "Player/PlayerCharacter.h"
#include "Components/ActorComponent.h"
#include "UserInterface/ToroWidgetManager.h"
#include "Helpers/ClassGetterMacros.h"
#include "QTEManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class FAFREVGAME_API UQTEManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UQTEManager();

	PLAYER_COMPONENT_GETTER(UQTEManager, APlayerCharacter, QuicktimeEvents)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UQTEManager* GetQuicktimeManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}

	UFUNCTION(BlueprintCallable, Category = QTEs)
		void InitiateEvent(UQTEInstance* InObject);

	UFUNCTION(BlueprintPure, Category = QTE, meta = (DeterminesOutputType = "Class"))
		const UQTEInstance* GetEventInstance(const TSubclassOf<UQTEInstance> Class);

	UFUNCTION(BlueprintPure, Category = QTEs)
		bool IsQuicktimeActive() const { return IsValid(ActiveQTE); }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuicktimeFinishedBP, const bool, bSuccess, TSubclassOf<UQTEInstance>&, QTE);
	UPROPERTY(BlueprintAssignable, DisplayName = "On QTE Finished")
		FOnQuicktimeFinishedBP OnQuicktimeFinishedBP;
	
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnQuicktimeFinished, const bool, const TSubclassOf<UQTEInstance>&);
	FOnQuicktimeFinished OnQuicktimeFinished;

	const UQTEInstance* GetInstanceObject() const { return ActiveQTE; }
	
protected:

	TObjectPtr<UQTEInstance> ActiveQTE;
	TObjectPtr<AToroWidgetManager> WidgetManager;

	void OnKeyPress(const FKey& Key) const;
	void QuicktimeFinished(const bool bSuccess);

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
