// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/Machine/States/WallRunInitState.h"

void UWallRunInitState::OnEnter()
{
	Super::OnEnter();
	GetWallRunContext()->LastWall = nullptr;
	TransitionTo(FallingStateName);
}

void UWallRunInitState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);
}

void UWallRunInitState::OnExit()
{
	Super::OnExit();
}
