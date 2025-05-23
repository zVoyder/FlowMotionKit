﻿// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/RunnableWall.h"

URunnableWall::URunnableWall(): WallGravityCurveOverride(nullptr),
                                WallSpeedAccelerationCurveOverride(nullptr),
                                bOverrideWallGravityScale(false),
                                bOverrideWallGravityCurve(false),
                                bOverrideWallSpeedAccelerationCurve(false),
                                bOverrideStickinessStrength(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool URunnableWall::HasWallGravityScaleOverride() const
{
	return bOverrideWallGravityScale;
}

bool URunnableWall::HasWallSpeedAccelerationCurveOverride() const
{
	return bOverrideWallSpeedAccelerationCurve && IsValid(WallSpeedAccelerationCurveOverride);
}

bool URunnableWall::HasWallGravityCurveOverride() const
{
	return bOverrideWallGravityCurve && IsValid(WallGravityCurveOverride);
}

bool URunnableWall::HasStickinessStrengthOverride() const
{
	return bOverrideStickinessStrength;
}
