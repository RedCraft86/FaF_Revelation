// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "SimpleDelegates.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FBoolStateDelegate, const bool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBoolStateDelegateBP, const bool, bState);