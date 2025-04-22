// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WallRunStateBase.h"
#include "WallRunInitState.generated.h"

UCLASS()
class FLOWMOTION_API UWallRunInitState : public UWallRunStateBase
{
	GENERATED_BODY()

protected:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;
};
