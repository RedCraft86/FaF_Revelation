// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

class FToroCoreModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
