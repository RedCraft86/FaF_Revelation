// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogToroUtilities, All, All);

class FToroUtilitiesModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
