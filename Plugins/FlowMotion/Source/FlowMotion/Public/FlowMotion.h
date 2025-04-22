// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogFlowMotion, All, All);

class FFlowMotionModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;
};
