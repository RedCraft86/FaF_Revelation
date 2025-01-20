// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroRuntime.h"

DEFINE_LOG_CATEGORY(LogToroRuntime);
UE_DEFINE_GAMEPLAY_TAG(Tag_Character, "Character");
UE_DEFINE_GAMEPLAY_TAG(Tag_Player, "Character.Player");
UE_DEFINE_GAMEPLAY_TAG(Tag_OneShot, "OneShot");
UE_DEFINE_GAMEPLAY_TAG(Tag_Zone, "Zone");
    
IMPLEMENT_MODULE(FToroRuntimeModule, ToroRuntime)