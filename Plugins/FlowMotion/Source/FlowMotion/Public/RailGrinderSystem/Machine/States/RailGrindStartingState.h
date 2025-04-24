// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RailGrindStateBase.h"
#include "RailGrindStartingState.generated.h"

UCLASS()
class FLOWMOTION_API URailGrindStartingState : public URailGrindStateBase
{
	GENERATED_BODY()

private:
	float ElapsedTime = 0.f;

protected:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;
};
