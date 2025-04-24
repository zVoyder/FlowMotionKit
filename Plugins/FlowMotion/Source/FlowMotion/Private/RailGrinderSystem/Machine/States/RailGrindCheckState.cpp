// Copyright VUEDK, Inc. All Rights Reserved.

#include "RailGrinderSystem/Machine/States/RailGrindCheckState.h"

void URailGrindCheckState::OnEnter()
{
	Super::OnEnter();
}

void URailGrindCheckState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);

	if (!HasSufficientSpeedToGrind())
		return;
	
	URailGrindContext* Context = GetRailGrindContext();
	if (Context->RailGrinder->TryGetMostValidRailHit(Context->HitData))
		TransitionTo(RaildGrindAttachmentStateName);
}

void URailGrindCheckState::OnExit()
{
	Super::OnExit();
}

bool URailGrindCheckState::HasSufficientSpeedToGrind() const
{
	const URailGrindContext* Context = GetRailGrindContext();
	const URailGrinder* RailGrinder = Context->RailGrinder;

	const float HorizontalSpeed = FVector(Context->MovementComponent->Velocity.X, Context->MovementComponent->Velocity.Y, 0.f).Length();
	return HorizontalSpeed > RailGrinder->MinSpeedToGrind;
}
