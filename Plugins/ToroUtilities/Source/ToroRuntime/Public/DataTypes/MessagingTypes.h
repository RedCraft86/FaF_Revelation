// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntimeSettings.h"
#include "MessagingTypes.generated.h"

USTRUCT(BlueprintType)
struct TORORUNTIME_API FSimpleMessageData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Message, meta = (MultiLine = true))
		FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Message)
		float ExtraTime;

	virtual ~FSimpleMessageData() = default;
	FSimpleMessageData() : Message(INVTEXT("")), ExtraTime(0.25f) {}
	FSimpleMessageData(const FText& InMessage, const float InTime) : Message(InMessage), ExtraTime(InTime) {}
	friend uint32 GetTypeHash(const FSimpleMessageData& Data)
	{
		return HashCombine(GetTypeHash(Data.Message.ToString()), GetTypeHash(Data.ExtraTime));
	}
	friend bool operator==(const FSimpleMessageData& A, const FSimpleMessageData& B)
	{
		return A.Message.ToString() == B.Message.ToString() && A.ExtraTime == B.ExtraTime;
	}
	friend bool operator!=(const FSimpleMessageData& A, const FSimpleMessageData& B) { return !(A == B); }
	
	FORCEINLINE virtual bool IsValidData() const { return !Message.IsEmptyOrWhitespace(); }
	FORCEINLINE virtual float GetDuration() const
	{
		return UToroRuntimeSettings::Get()->GetReadingTime(Message.ToString()) + ExtraTime;
	}
};

USTRUCT(BlueprintType)
struct TORORUNTIME_API FSimpleSubtitleData : public FSimpleMessageData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Message, meta = (DisplayPriority = -1))
		FText Speaker;

	FSimpleSubtitleData() : Speaker(INVTEXT("You")) { ExtraTime = 1.0f; }
	FSimpleSubtitleData(const FText& InSpeaker, const FText& InLine, const float InTime = 1.0f) : Speaker(InSpeaker)
	{
		Message = InLine;
		ExtraTime = InTime;
	}
	friend uint32 GetTypeHash(const FSimpleSubtitleData& Data)
	{
		return HashCombine(HashCombine(GetTypeHash(Data.Message.ToString()),
				GetTypeHash(Data.ExtraTime)), GetTypeHash(Data.Speaker.ToString()));
	}
	friend bool operator==(const FSimpleSubtitleData& A, const FSimpleSubtitleData& B)
	{
		return A.Speaker.ToString() == B.Speaker.ToString()
			&& A.Message.ToString() == B.Message.ToString()
			&& A.ExtraTime == B.ExtraTime;
	}
	friend bool operator!=(const FSimpleSubtitleData& A, const FSimpleSubtitleData& B) { return !(A == B); }
	
	FORCEINLINE virtual bool IsValidData() const override
	{
		return !Speaker.IsEmptyOrWhitespace() && !Message.IsEmptyOrWhitespace();
	}
	FORCEINLINE virtual float GetDuration() const override
	{
		return UToroRuntimeSettings::Get()->GetReadingTime(Message.ToString()) + ExtraTime;
	}
};
