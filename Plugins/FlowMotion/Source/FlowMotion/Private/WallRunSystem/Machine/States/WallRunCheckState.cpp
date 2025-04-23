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

	const UWallRunner* Runner = Context->Runner;
	FWallHitData HitData;
	if (HasSufficientSpeedToAttach() && Runner->TryGetMostValidWallHit(HitData))
	{
		if (!CanAttachToWall(HitData))
			return;

		Context->HitData = HitData;
		TransitionTo(AttachStateName);
	}
}

void UWallRunCheckState::OnExit()
{
	Super::OnExit();
}

bool UWallRunCheckState::CanAttachToWall(const FWallHitData& WallHitData) const
{
	if (GetWallRunContext()->Runner->bAllowsMultipleAttachOnSameWall)
		return true;
	
	return WallHitData != GetWallRunContext()->HitData;
}

bool UWallRunCheckState::HasSufficientSpeedToAttach() const
{
	const FVector HorizontalVelocity = FVector(
		GetWallRunContext()->MovementComponent->Velocity.X,
		GetWallRunContext()->MovementComponent->Velocity.Y,
		0.f
	);

	return HorizontalVelocity.Length() >= GetWallRunContext()->Runner->VelocityToAttach;
}

