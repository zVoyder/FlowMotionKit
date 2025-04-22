// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MotionStateMachine/MotionState.h"
#include "TestingState.generated.h"

UCLASS()
class FLOWMOTION_API UTestingState : public UMotionState
{
	GENERATED_BODY()

protected:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;
};
