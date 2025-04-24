// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RailGrindStateBase.h"
#include "RailGrindGrindingState.generated.h"

UCLASS()
class FLOWMOTION_API URailGrindGrindingState : public URailGrindStateBase
{
	GENERATED_BODY()

private:
	float DistanceAlongSpline;
	bool bIsMovingForward;
	
protected:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;

	virtual void OnAbort() override;

private:
	void UpdateDistanceAlongSpline();
};
