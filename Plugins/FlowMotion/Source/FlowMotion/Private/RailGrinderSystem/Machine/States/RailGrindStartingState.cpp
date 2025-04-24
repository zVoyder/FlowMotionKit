// Copyright VUEDK, Inc. All Rights Reserved.

#include "RailGrinderSystem/Machine/States/RailGrindStartingState.h"

void URailGrindStartingState::OnEnter()
{
	Super::OnEnter();
	ElapsedTime = 0.f;
	GetRailGrindContext()->HitData.Rail = nullptr;
}

void URailGrindStartingState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);
	ElapsedTime += DeltaTime;
	
	if (ElapsedTime > GetRailGrindContext()->RailGrinder->AttachInputDelay)
		TransitionTo(RailGrindCheckStateName);
}

void URailGrindStartingState::OnExit()
{
	Super::OnExit();
}
