// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WallRunStateBase.h"
#include "WallRunFallingState.generated.h"

UCLASS()
class FLOWMOTION_API UWallRunFallingState : public UWallRunStateBase
{
	GENERATED_BODY()

private:
	float Timer = 0.f;
	
protected:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;
};
