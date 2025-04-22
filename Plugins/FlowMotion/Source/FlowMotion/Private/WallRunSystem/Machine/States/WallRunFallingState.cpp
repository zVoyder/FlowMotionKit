// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/Machine/States/WallRunFallingState.h"

void UWallRunFallingState::OnEnter()
{
	Super::OnEnter();
	Timer = 0.f;
}

void UWallRunFallingState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);
	Timer += DeltaTime;
	
	if (Timer >= GetWallRunContext()->Runner->TimeBeforeAttach)
		TransitionTo(InputStateName);
}

void UWallRunFallingState::OnExit()
{
	Super::OnExit();
	const UCharacterMovementComponent* MovementComponent = GetWallRunContext()->MovementComponent;
	GetWallRunContext()->StartUpHorVelocity = FVector(MovementComponent->Velocity.X, MovementComponent->Velocity.Y, 0.f);
}
