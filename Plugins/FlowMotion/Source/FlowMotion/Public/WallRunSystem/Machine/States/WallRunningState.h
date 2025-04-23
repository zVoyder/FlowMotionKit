// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WallRunStateBase.h"
#include "WallRunningState.generated.h"

constexpr float TraceDistanceMultiplier = 1.5f;

UCLASS()
class FLOWMOTION_API UWallRunningState : public UWallRunStateBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UCurveFloat* GravityCurve;
	UPROPERTY()
	UCurveVector* SpeedAccelerationCurve;
	float ElapsedTime = 0.f;
	float ContactSpeed = 0.f;
	float ContactAcceleration = 0.f;
	bool bUseGravityCurve = false;
	bool bUseSpeedAccelerationCurve = false;

protected:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;

	virtual void OnAbort() override;

private:
	bool HasWallOnSide() const;

	bool HasSufficientSpeedToKeepRunning() const;
	
	void SetGravity();

	void ScaleGravityWithCurve() const;

	void SetSpeedAcceleration();

	void ScaleSpeedAccelerationWithCurve() const;

	void LaunchCharacterOffWall() const;
};
