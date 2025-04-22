// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/RunnableWall.h"

URunnableWall::URunnableWall(): WallGravityMultiplierCurve(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}
