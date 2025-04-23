// Copyright VUEDK, Inc. All Rights Reserved.

#include "WallRunSystem/Machine/States/WallRunAttachmentState.h"

void UWallRunAttachmentState::OnEnter()
{
	Super::OnEnter();
	ElapsedTime = 0.f;
	
	GetWallRunContext()->MovementComponent->GravityScale = 0.f;
	GetWallRunContext()->MovementComponent->bOrientRotationToMovement = false;
	SetVerticalVelocity(0.f);
}

void UWallRunAttachmentState::OnProcess(float DeltaTime)
{
	Super::OnProcess(DeltaTime);
	ElapsedTime += DeltaTime;

	const UWallRunner* Runner = GetWallRunContext()->Runner;
	UWallRunContext* Context = GetWallRunContext();

	if (!Runner->WantsToAttach() || !Runner->TryGetMostValidWallHit(Context->HitData))
	{
		TransitionTo(FallingStateName);
		return;
	}
	
	const FRotator WallOrientation = Runner->GetWallOrientation(Context->HitData);
	Runner->RotateCharacterAlongWall(DeltaTime, WallOrientation);
	const float Stickiness = Runner->GetStickinessStrength(Context->HitData.Wall);
	Runner->MoveCharacterAlongWall(DeltaTime, Context->HitData.HitResult, WallOrientation, Stickiness);
	
	if (ElapsedTime >= Runner->AttachmentDuration)
		TransitionTo(RunningStateName);
}

void UWallRunAttachmentState::OnAbort()
{
	Super::OnAbort();
	GetWallRunContext()->Runner->ResetMovementComponentData(); // Safe call
}

void UWallRunAttachmentState::SetVerticalVelocity(const float ZVelocity) const
{
	GetWallRunContext()->MovementComponent->Velocity = FVector(GetWallRunContext()->MovementComponent->Velocity.X, GetWallRunContext()->MovementComponent->Velocity.Y, ZVelocity);
}
