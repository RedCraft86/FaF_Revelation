// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditorCommands.h"
#include "Framework/Commands/Commands.h"

class FActorLayout
{
	friend class SActorLayoutDialog;

public:

	static void Register(const TSharedPtr<FUICommandList>& Commands)
	{
		MAP_TOOL(ActorLayout, FActorLayout);
	}
	
	static void RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
	{
		REGISTER_TOP_MENU(ActorLayout)
		REGISTER_TOOLBAR(ActorLayout, Actor)
	}

private:

	static inline int32 MinX = 1000;
	static float GetMinX() { return MinX; }
	static void SetMinX(float InValue)
	{
		MinX = FMath::RoundToInt(FMath::Max(1.0f, InValue));
	}

	static inline int32 MinY = 1000;
	static float GetMinY() { return MinY; }
	static void SetMinY(float InValue)
	{
		MinY = FMath::RoundToInt(FMath::Max(1.0f, InValue));
	}

	static inline int32 Yaw = 0;
	static float GetYaw() { return Yaw; }
	static void SetYaw(float InValue)
	{
		Yaw = FMath::RoundToInt(FMath::Clamp(InValue, -90.0f, 90.0f));
	}

	static inline uint8 PerRow = 5;
	static float GetPerRow() { return PerRow; }
	static void SetPerRow(float InValue)
	{
		PerRow = FMath::RoundToInt(FMath::Max(1.0f, InValue));
	}
	
	static inline TSharedPtr<SWindow> Window = nullptr;
	static void ExecuteAction();
	static void LayoutActors();
};

class SButton;

class SActorLayoutDialog : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SActorLayoutDialog){}
	SLATE_END_ARGS()

	SActorLayoutDialog() {}
	void Construct(const FArguments& InArgs);

private:

	FReply OnLayoutClicked();
};