// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroRuntime.h"

DEFINE_LOG_CATEGORY(LogToroRuntime);

UE_DEFINE_GAMEPLAY_TAG(Tag_BGs, "Backgrounds");
UE_DEFINE_GAMEPLAY_TAG(Tag_Zone, "Zone");
UE_DEFINE_GAMEPLAY_TAG(Tag_LocalSound, "LocalSound");
UE_DEFINE_GAMEPLAY_TAG(Tag_Character, "Character");
UE_DEFINE_GAMEPLAY_TAG(Tag_Player, "Character.Player");
UE_DEFINE_GAMEPLAY_TAG(Tag_Friendly, "Character.Friendly");
UE_DEFINE_GAMEPLAY_TAG(Tag_Enemy, "Character.Enemy");
    
IMPLEMENT_MODULE(FToroRuntimeModule, ToroRuntime)