// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Internationalization/Text.h"
#include "MessagingTypes.generated.h"

USTRUCT(BlueprintType)
struct FGenericMessage
{
	GENERATED_BODY()

	virtual ~FGenericMessage() = default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Message)
		FText Content;

	virtual bool IsValid() const
	{
		return !Content.IsEmptyOrWhitespace();
	}

	virtual float GetReadTime() const
	{
		return static_cast<float>(Content.ToString().Len()) / CharsPerSecond;
	}

protected:

	static constexpr float CharsPerSecond = 10.0f;
};

USTRUCT(BlueprintType)
struct FSubtitleMessage final : public FGenericMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Message)
		FText Speaker;

	virtual bool IsValid() const override
	{
		return Super::IsValid() && !Speaker.IsEmptyOrWhitespace();
	}

	virtual float GetReadTime() const override
	{
		return Super::GetReadTime() + static_cast<float>(Speaker.ToString().Len()) / CharsPerSecond;
	}
};
