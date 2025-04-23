// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveVector.h"
#include "RunnableWall.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLOWMOTION_API URunnableWall : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallRun", meta = (EditCondition = "bOverrideWallGravityCurve"))
	UCurveFloat* WallGravityCurveOverride;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallRun", meta = (EditCondition = "bOverrideWallSpeedAccelerationCurve"))
	UCurveVector* WallSpeedAccelerationCurveOverride;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallRun", meta = (EditCondition = "bOverrideStickinessStrength"), AdvancedDisplay)
	float StickinessStrengthOverride = 4.f;

private:
	UPROPERTY()
	bool bOverrideWallGravityCurve;
	UPROPERTY()
	bool bOverrideWallSpeedAccelerationCurve;
	UPROPERTY()
	bool bOverrideStickinessStrength;
	
public:
	URunnableWall();

	/**
	 * @brief Checks if the wall has a gravity curve override.
	 * @return True if the wall has a gravity curve override, false otherwise.
	 */
	bool HasWallSpeedAccelerationCurveOverride() const;

	/**
	 * @brief Checks if the wall has a speed and acceleration curve override.
	 * @return True if the wall has a speed and acceleration curve override, false otherwise.
	 */
	bool HasWallGravityCurveOverride() const;

	/**
	 * @brief Checks if the wall has a stickiness strength override.
	 * @return True if the wall has a stickiness strength override, false otherwise.
	 */
	bool HasStickinessStrengthOverride() const;
};
