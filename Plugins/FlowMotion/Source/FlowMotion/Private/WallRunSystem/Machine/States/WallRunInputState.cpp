// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/Machine/States/WallRunInputState.h"

void UWallRunInputState::OnEnter()
{
	Super::OnEnter();
}

void UWallRunInputState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);

	if (GetWallRunContext()->Runner->WantsToAttach())
	{
		TransitionTo(CheckStateName);
		return;
	}
}

void UWallRunInputState::OnExit()
{
	Super::OnExit();
}
