// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WallRunStateBase.h"
#include "WallRunRunningState.generated.h"

constexpr float TraceDistanceMultiplier = 1.5f;

UCLASS()
class FLOWMOTION_API UWallRunRunningState : public UWallRunStateBase
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
	/**
	 * @brief Detaches the character from the wall and launches it.
	 */
	void DetachAndLaunchCharacter() const;

	/**
	 * @brief Detaches the character from the wall.
	 */
	void Detach() const;

	/**
	 * @brief Checks if the character has a wall on the side.
	 * @return True if the character has a wall on the side, false otherwise.
	 */
	bool HasWallOnSide() const;

	/**
	 * @brief Checks if the character has sufficient speed to keep running on the wall.
	 * @return True if the character has sufficient speed to keep running, false otherwise.
	 */
	bool HasSufficientSpeedToKeepRunning() const;

	/**
	 * @brief Sets the gravity scale of the character for wall running.
	 */
	void SetGravity();

	/**
	 * @brief Scales the gravity of the character based on the curve.
	 */
	void ScaleGravityWithCurve() const;

	/**
	 * @brief Sets the speed and acceleration of the character for wall running.
	 */
	void SetSpeedAcceleration();

	/**
	 * @brief Scales the speed and acceleration of the character based on the curve.
	 */
	void ScaleSpeedAccelerationWithCurve() const;

	/**
	 * @brief Launches the character off the wall.
	 */
	void LaunchCharacterOffWall() const;
};
