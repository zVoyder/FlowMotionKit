// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/Machine/States/WallRunInitState.h"

void UWallRunInitState::OnEnter()
{
	Super::OnEnter();
	GetWallRunContext()->HitData.Wall = nullptr;
	TransitionTo(WallRunFallingStateName);
}

void UWallRunInitState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);
}

void UWallRunInitState::OnExit()
{
	Super::OnExit();
}
