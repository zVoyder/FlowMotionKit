// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/Machine/States/WallRunFallingState.h"

void UWallRunFallingState::OnEnter()
{
	Super::OnEnter();
	Timer = 0.f;
	GetWallRunContext()->Runner->ResetMovementComponentData();
}

void UWallRunFallingState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);
	Timer += DeltaTime;
	
	if (Timer >= GetWallRunContext()->Runner->AttachInputDelay)
		TransitionTo(WallRunInputStateName);
}

void UWallRunFallingState::OnExit()
{
	Super::OnExit();
}
