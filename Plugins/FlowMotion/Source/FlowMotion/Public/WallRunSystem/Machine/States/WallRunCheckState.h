// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WallRunStateBase.h"
#include "WallRunCheckState.generated.h"

UCLASS()
class FLOWMOTION_API UWallRunCheckState : public UWallRunStateBase
{
	GENERATED_BODY()

protected:
	virtual void OnEnter() override;
	
	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;

	virtual void OnAbort() override;
};
