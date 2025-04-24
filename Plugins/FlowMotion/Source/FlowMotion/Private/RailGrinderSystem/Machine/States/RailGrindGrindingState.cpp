// Copyright VUEDK, Inc. All Rights Reserved.

#include "RailGrinderSystem/Machine/States/RailGrindGrindingState.h"

void URailGrindGrindingState::OnEnter()
{
	Super::OnEnter();
	const URailGrindContext* Context = GetRailGrindContext();
	UpdateDistanceAlongSpline();
}

void URailGrindGrindingState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);

	URailGrindContext* Context = GetRailGrindContext();
	URailGrinder* RailGrinder = Context->RailGrinder;
	
	RailGrinder->MoveAndRotateCharacterAlongRail(DeltaTime, DistanceAlongSpline, Context->HitData, Context->bIsGoingReverse);
}

void URailGrindGrindingState::OnExit()
{
	Super::OnExit();
}

void URailGrindGrindingState::OnAbort()
{
	Super::OnAbort();
	GetRailGrindContext()->RailGrinder->ResetMovementComponentData();
	// TODO: Launch character off rail here, because we stop the state machine while we are grinding
}

void URailGrindGrindingState::UpdateDistanceAlongSpline()
{
	const URailGrindContext* Context = GetRailGrindContext();
	DistanceAlongSpline = Context->HitData.Rail->GetClosestDistanceOnSpline(Context->HitData.HitResult.ImpactPoint);
}
