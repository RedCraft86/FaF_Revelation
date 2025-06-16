// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GlobalEventPayload.h"
#include "Helpers/ClassGetters.h"
#include "Helpers/GameplayTagMacros.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GlobalEventManager.generated.h"

namespace GlobalEvents
{
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(GlobalEvent)
	TOROUTILITIES_API CREATE_TAG_VERIFIER(GlobalEvent)
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FGlobalEventSingle, const UGlobalEventPayload*, Payload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGlobalEventMulti, const UGlobalEventPayload*, Payload);

UCLASS(BlueprintType, DisplayName = "Global Events")
class TOROUTILITIES_API UGlobalEventManager final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UGlobalEventManager() {}
	GAME_INSTANCE_SUBSYSTEM_GETTER(UGlobalEventManager);

	/* Binds a delegate to a global event. The delegate will be called whenever this event is invoked. */
	UFUNCTION(BlueprintCallable, Category = GlobalEvents)
		void BindGlobalEvent(UPARAM(meta = (Categories = "GlobalEvent")) const FGameplayTag EventID, const FGlobalEventSingle& Event);

	/* Unbinds a delegate from the specified global event.
	 * If no delegates are bound, the event will be removed, and its cached payload will no longer be retained.
	 */
	UFUNCTION(BlueprintCallable, Category = GlobalEvents)
		void UnbindGlobalEvent(UPARAM(meta = (Categories = "GlobalEvent")) const FGameplayTag EventID, const FGlobalEventSingle& Event);

	/* Invokes a global event by tag. Passing in a payload is required to pass the Instigator.
	 * The payload will be cached by the manager to avoid garbage collection and remain accessible to listeners.
	 */
	UFUNCTION(BlueprintCallable, Category = GlobalEvents, meta = (AdvancedDisplay = "Instigator", DefaultToSelf = "Instigator"))
		void InvokeGlobalEvent(UPARAM(meta = (Categories = "GlobalEvent")) const FGameplayTag EventID, UGlobalEventPayload* Payload, UObject* Instigator);

	/* Completely removes the specified global event from the system.
	 * This also clears any bound delegates and releases the cached payload associated with the event.
	 */
	UFUNCTION(BlueprintCallable, Category = GlobalEvents)
		void RemoveGlobalEvent(UPARAM(meta = (Categories = "GlobalEvent")) const FGameplayTag EventID);

	/* Gets the payload associated to an event. */
	UFUNCTION(BlueprintCallable, Category = GlobalEvents)
	UGlobalEventPayload* GetEventPayload(const FGameplayTag& EventID)
	{
		return Payloads.Contains(EventID) ? *Payloads.Find(EventID) : nullptr;
	}

	FGlobalEventMulti& GetEventDelegate(const FGameplayTag& EventID) { return Events.FindOrAdd(EventID); }

private:

	UPROPERTY(Transient) TMap<FGameplayTag, FGlobalEventMulti> Events;
	UPROPERTY(Transient) TMap<FGameplayTag, TObjectPtr<UGlobalEventPayload>> Payloads;
};
