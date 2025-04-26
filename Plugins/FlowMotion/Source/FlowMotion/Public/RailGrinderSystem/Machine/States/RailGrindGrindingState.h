// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RailGrindStateBase.h"
#include "RailGrindGrindingState.generated.h"

UCLASS()
class FLOWMOTION_API URailGrindGrindingState : public URailGrindStateBase
{
	GENERATED_BODY()

protected:
	virtual void OnEnter() override;

	virtual void OnProcess(float DeltaTime) override;

	virtual void OnExit() override;

	virtual void OnAbort() override;

private:
	/**
	 * @brief Updates the distance along the spline based on the rail hit data.
	 */
	void UpdateDistanceAlongSpline() const;

	/**
	 * @brief Launches the character off the rail.
	 */
	void LaunchCharacterOffRail() const;

	/**
	 * @brief Checks if the character has reached the end of the rail.
	 * @return True if the character has reached the end of the rail, false otherwise.
	 */
	bool HasReachedEndOfRail() const;

	/**
	 * @brief Gets the distance at which the character should detach from the rail.
	 * @return The distance at which the character should detach from the rail.
	 */
	float GetDetachDistance() const;
};
