// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "DataTypes/MessagingTypes.h"
#include "Components/ActorComponent.h"
#include "MessageManager.generated.h"

UENUM(BlueprintType)
enum class EMessageInsertionType : uint8
{
	InsertLast,
	InsertNow,
	Replace,
	Override
};

UCLASS(NotBlueprintable, BlueprintType)
class UMessageManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UMessageManager();

	UFUNCTION(BlueprintPure, Category = Messaging, meta = (WorldContext = ContextObject, DisplayName = "Get Message Manager"))
		static UMessageManager* Get(const UObject* ContextObject);

	UFUNCTION(BlueprintCallable, Category = Messaging)
		void QueueNotice(const FGenericMessage& Entry, const EMessageInsertionType InsertionType);

	UFUNCTION(BlueprintCallable, Category = Messaging)
		void QueueNotices(const TArray<FGenericMessage>& Entries, const EMessageInsertionType InsertionType);

	UFUNCTION(BlueprintCallable, Category = Messaging)
		void QueueSubtitle(const FSubtitleMessage& Entry, const EMessageInsertionType InsertionType);

	UFUNCTION(BlueprintCallable, Category = Messaging)
		void QueueSubtitles(const TArray<FSubtitleMessage>& Entries, const EMessageInsertionType InsertionType);

	DECLARE_DELEGATE_OneParam(FOnNoticeText, const FGenericMessage&);
	FOnNoticeText OnNoticeText;

	DECLARE_DELEGATE_OneParam(FOnSubtitleText, const FSubtitleMessage&);
	FOnSubtitleText OnSubtitleText;

private:

	UPROPERTY(Transient)
		TObjectPtr<class AGamePlayerController> PlayerController;

	TArray<TPair<FGenericMessage, float>> Notices;
	TArray<TPair<FSubtitleMessage, float>> Subtitles;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
};
