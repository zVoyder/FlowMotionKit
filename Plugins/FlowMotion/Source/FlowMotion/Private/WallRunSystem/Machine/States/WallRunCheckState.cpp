// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/Machine/States/WallRunCheckState.h"
#include "WallRunSystem/Machine/States/WallRunInputState.h"

void UWallRunCheckState::OnEnter()
{
	Super::OnEnter();
}

void UWallRunCheckState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);

	UWallRunContext* Context = GetWallRunContext();
	
	if (!Context->Runner->WantsToAttach())
	{
		TransitionTo(InputStateName);
		return;
	}
	
	URunnableWall* CurrentWall = nullptr;
	FHitResult HitResult;
	
	if (Context->Runner->CheckAttachWall(Context->StartUpHorVelocity.Length(), true, HitResult, CurrentWall))
	{
		if (CurrentWall == Context->LastWall)
			return;
	
		Context->LastWall = CurrentWall;
		Context->bLastWallIsRight = true;
		TransitionTo(RunningStateName);
		return;
	}
	
	if (Context->Runner->CheckAttachWall(Context->StartUpHorVelocity.Length(), false, HitResult, CurrentWall))
	{
		if (CurrentWall == Context->LastWall)
			return;
	
		Context->LastWall = CurrentWall;
		Context->bLastWallIsRight = false;
		TransitionTo(RunningStateName);
		return;
	}
}

void UWallRunCheckState::OnExit()
{
	Super::OnExit();
}

void UWallRunCheckState::OnAbort()
{
	Super::OnAbort();
}
